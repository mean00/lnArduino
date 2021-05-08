/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnSPI.h"
extern "C"
{
#include "gd32vf103_spi.h"
}
#include "lnSPI_priv.h"

struct SpiDescriptor
{
    uint32_t    spiAddress;
    IRQn_Type   spiIrq;
    int         dmaEngine;
    int         dmaTxChannel;
    rcu_periph_enum rcu;
    uint32_t    bank, mosi,miso, clk;
};

// We assume all the pins of a given SPI are on the same gpio bank
static const SpiDescriptor spiDescriptor[3]=
{
    //             DMA DMATX CLOCK   GPIO MOSI MISO CLK
    {SPI0, SPI0_IRQn,0, 2, RCU_SPI0, GPIOA,PA7, PA6, PA5},
    {SPI1, SPI1_IRQn,0, 4, RCU_SPI1, GPIOB,PB15,PB14,PB13},
    {SPI2, SPI2_IRQn,1, 1, RCU_SPI2, GPIOB,PB5, PB4, PB3}
};

/**
 * switch between RX/TX and TX only : false is txRx, 
 * @param adr
 * @param rxTx
 */
static void updateMode(uint32_t adr,bool rxTx)
{

    uint32_t reg=SPI_CTL0(adr);
    reg&=~(SPI_CTL0_BDOEN | SPI_CTL0_BDEN | SPI_CTL0_RO);
    if(rxTx)
    {
        reg|=0; // 0 all bits = full duplex
    }else
    {   // Tx Only: unidirectional, transmit only, 
        reg|=SPI_CTL0_BDEN | SPI_CTL0_BDOEN;
    }
    SPI_CTL0(adr)=reg;

}
/**
 * 
 * @param adr
 * @param bits
 */
void updateDataSize(uint32_t adr,int bits)
{
    uint32_t reg=SPI_CTL0(adr);
    reg&=~SPI_CTL0_SPIEN;
    SPI_CTL0(adr)=reg;
    
    switch(bits)
    {
        case 8:
             reg&=~SPI_CTL0_FF16;
             break;
        case 16:
            reg|=SPI_FRAMESIZE_16BIT;
            break;
        default:xAssert(0);
    }
    reg|=SPI_CTL0_SPIEN;
    SPI_CTL0(adr)=reg;
}
void updateDmaTX(uint32_t adr,bool onoff)
{
    uint32_t reg=SPI_CTL0(adr);
    reg&=~SPI_CTL0_SPIEN;
    SPI_CTL0(adr)=reg;
    if(onoff)
         SPI_CTL1(adr) |= (uint32_t)(SPI_CTL1_DMATEN);
    else
         SPI_CTL1(adr) &= ~((uint32_t)(SPI_CTL1_DMATEN));
    reg|=SPI_CTL0_SPIEN;
    SPI_CTL0(adr)=reg;
}

/**
 * 
 * @param instance
 * @param pinCs
 */
#define M(x) spiDescriptor[instance].x

hwlnSPIClass::hwlnSPIClass(int instance, int pinCs) : _internalSettings(1000000,SPI_MSBFIRST,SPI_MODE0,-1),_currentSetting(1000000,SPI_MSBFIRST,SPI_MODE0,-1),
        txDma(lnDMA::DMA_MEMORY_TO_PERIPH,M(dmaEngine),M(dmaTxChannel),16,16)
{
    _useDMA=false;
    _cookie=NULL;
    _callback=NULL;
    _instance=instance;
    _settings=NULL;
    
    xAssert(instance<3);
    const SpiDescriptor *d=spiDescriptor+instance;
    
    _irq=d->spiIrq;
    _adr=d->spiAddress;
    rcu_periph_clock_enable(d->rcu);
    // setup miso/mosi & clk
    gpio_init(d->bank, GPIO_MODE_AF_PP,         GPIO_OSPEED_50MHZ, digitalPinToBitMask(d->mosi));
    gpio_init(d->bank, GPIO_MODE_IN_FLOATING,   GPIO_OSPEED_50MHZ, digitalPinToBitMask(d->miso));
    gpio_init(d->bank, GPIO_MODE_AF_PP,         GPIO_OSPEED_50MHZ, digitalPinToBitMask(d->clk));
    
    if(pinCs!=-1)
    {
        pinMode(pinCs,OUTPUT);
        digitalWrite(pinCs,HIGH);
    }
    spi_i2s_deinit(_adr);    
}
/**
 * 
 */
hwlnSPIClass::~hwlnSPIClass()
{
     spi_i2s_deinit(_adr);    
}
/**
 * 
 */
void hwlnSPIClass::begin()
{    
    _settings=&_internalSettings;
    spi_enable(_adr);
    setup();
}
/**
 * 
 */
void hwlnSPIClass::end(void)
{
    spi_disable(_adr);
}    


void hwlnSPIClass::beginTransaction(lnSPISettings &settings)
{
    _mutex.lock();
    _currentSetting=settings;
    _settings=&settings;
    
    // setup
    setup();
}
/**
 */
void hwlnSPIClass::endTransaction()
{
    _settings=&_internalSettings;
    setup(); // restore settings
    _mutex.unlock();
}
/**
 * 
 * @param nbBytes
 * @param data
 * @param callback
 * @return 
 */
bool hwlnSPIClass::asyncWrite(int nbBytes, const uint8_t *data,lnSpiCallback *callback,void *cookie)
{
    xAssert(callback);
    _callback=callback;
    _callbackCookie=cookie;
    return write(nbBytes,data);
}
/**
 * 
 * @param nbBytes
 * @param dataOut
 * @param dataIn
 * @param callback
 * @return 
 */
bool hwlnSPIClass::asyncTransfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn,lnSpiCallback *callback,void *cookie)
{
    xAssert(callback);
    _callback=callback;
    _callbackCookie=cookie;
    return transfer(nbBytes,dataOut,dataIn);
}
/**
 * 
 * @param z
 * @return 
 */
bool hwlnSPIClass::write(int z)
{
    csOn();
    updateDataSize(_adr,8);
    updateMode(_adr,false);
    
    while (!spi_i2s_flag_get(_adr, SPI_FLAG_TBE)) 
    {
    }
    spi_i2s_data_transmit(_adr, z);
    waitForCompletion();
    csOff();
    return true;
}
/**
 * 
 * @param z
 * @return 
 */
bool hwlnSPIClass::write16(int z)
{
    updateMode(_adr,false); // Tx only
    updateDataSize(_adr,16);
    csOn();    
    while (!spi_i2s_flag_get(_adr, SPI_FLAG_TBE)) 
    {
    }
    spi_i2s_data_transmit(_adr, z);
    waitForCompletion();
    csOff();
    return true;
}

/**
 */
bool hwlnSPIClass::write(int nbBytes, const uint8_t *data)
{
    updateMode(_adr,false);
    updateDataSize(_adr,8);
    csOn();
    for (size_t i = 0; i < nbBytes; i++) 
    {
        while (!spi_i2s_flag_get(_adr, SPI_FLAG_TBE)) 
        {
        }
        spi_i2s_data_transmit(_adr, data[i]);
    }
    waitForCompletion();
    csOff();
    return true;
}
/**
 */
bool hwlnSPIClass::dmaWrite(int nbBytes, const uint8_t *data)
{
    updateMode(_adr,false);
    updateDataSize(_adr,8);
    csOn();
    for (size_t i = 0; i < nbBytes; i++) 
    {
        while (!spi_i2s_flag_get(_adr, SPI_FLAG_TBE)) 
        {
        }
        spi_i2s_data_transmit(_adr, data[i]);
    }
    waitForCompletion();
    csOff();
    return true;
}

/**
 */
bool hwlnSPIClass::dmaWrite16(int nbWord, const uint16_t *data)
{
    updateMode(_adr,false); // tx only
    updateDataSize(_adr,16);// 16 bits at a time
    
    csOn();
    Logger(">>1\n");
    txDma.doMemoryToPeripheralTransfer(nbWord, data, (uint16_t *)&SPI_DATA(_adr),false);        
    updateDmaTX(_adr,true); // activate DMA
    Logger("++1\n");
    _done.take();
    Logger("<<1\n");
    waitForCompletion();
    
    csOff();
    updateDmaTX(_adr,false);
    return true;
}



/**
 */
bool hwlnSPIClass::dmaWrite16Repeat(int nbWord, const uint16_t data)
{
    updateMode(_adr,false);
    updateDataSize(_adr,16);
    updateDmaTX(_adr,true);
    csOn();
    
    Logger(">>2\n");
    
    txDma.doMemoryToPeripheralTransfer(nbWord, &data, (uint16_t *)&SPI_DATA(_adr), true);        
    Logger("++2\n");
    _done.take();
    Logger("<<2\n");
    waitForCompletion();
    
    csOff();
    updateDmaTX(_adr,false);
    return true;
}
/**
 * 
 */
void hwlnSPIClass::csOn()
{
    if(_settings->pinCS!=-1)
    {
        digitalWrite(_settings->pinCS,LOW);
    }
}
/**
 * 
 */
void hwlnSPIClass::csOff()
{
    if(_settings->pinCS!=-1)
    {
        digitalWrite(_settings->pinCS,HIGH);
    }
}
/**
 *  This is needed to be able to toggle the CS when all is done
 */
void hwlnSPIClass::waitForCompletion()
{
        while (spi_i2s_flag_get(_adr, SPI_FLAG_TRANS)) 
        {
        }
}
/**
 */
bool hwlnSPIClass::transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn)
{
    updateMode(_adr,true);
    updateDataSize(_adr,8);
    csOn();
    for (size_t i = 0; i < nbBytes; i++) 
    {
        while (!spi_i2s_flag_get(_adr, SPI_FLAG_TBE)) 
        {
        }
        spi_i2s_data_transmit(_adr, dataOut[i]);
        while (!spi_i2s_flag_get(_adr, SPI_FLAG_RBNE)) 
        {        
        }
        dataIn[i] = spi_i2s_data_receive(_adr);
    }
    waitForCompletion();
    csOff();
    return true;
}
/**
 * 
 * @param c
 */
void hwlnSPIClass::exTxDone(void *c)
{
    hwlnSPIClass *i=(hwlnSPIClass *)c;
    i->txDone();
}
/**
 * 
 * @return 
 */
void hwlnSPIClass::txDone()
{
    _done.give();
}
/**
 * \brief program the peripheral with the current settings
 */
void hwlnSPIClass::setup()
{
    xAssert(_settings);
    
    
    spi_parameter_struct param;
    spi_struct_para_init(&param);
    
    param.device_mode=SPI_MASTER;
    param.frame_size=SPI_FRAMESIZE_8BIT;
    param.nss=SPI_NSS_SOFT;
    
    
    switch(_settings->bOrder)
    {
        case   SPI_LSBFIRST: param.endian=SPI_ENDIAN_LSB;break;
        case   SPI_MSBFIRST:param.endian=SPI_ENDIAN_MSB;break;
        default:xAssert(0);
                break;            
    }
    switch(_settings->dMode)
    {
        case SPI_MODE0:
            param.clock_polarity_phase=SPI_CK_PL_LOW_PH_1EDGE;
            break;
        case SPI_MODE1:
            param.clock_polarity_phase=SPI_CK_PL_LOW_PH_2EDGE;
            break;
        case SPI_MODE2:
            param.clock_polarity_phase=SPI_CK_PL_HIGH_PH_1EDGE;
            break;
        case SPI_MODE3:
            param.clock_polarity_phase=SPI_CK_PL_HIGH_PH_2EDGE;
            break;
        default:xAssert(0);
                break;
    }     
    uint32_t prescale = 0,speed=_settings->speed,apb ;
    xAssert(speed);
    if (!_instance ) 
    {
        apb=rcu_clock_freq_get(CK_APB2);
    }
    else 
    {
        apb=rcu_clock_freq_get(CK_APB1);
    }
    prescale = (apb*1000000) / speed- 1;
    // prescale can only go from 2 to 256, and prescale=2^(psc+1) actually
    
    uint32_t psc=0;
    if(prescale<=2) psc=0;
    else if(prescale>=256) psc=7;
    else
    {
        psc=0;
        uint32_t toggle=2;
        while(prescale>toggle)
        {
            toggle*=2;
            psc++;
        }
    }
    
    param.prescale = psc;;
    param.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init(_adr, &param);
    txDma.attachCallback(exTxDone,this);
    spi_enable(_adr);  
}
/**
 * 
 * @param dataSize
 */
void hwlnSPIClass::setDataSize(int dataSize)
{
    updateDataSize(_adr,dataSize);
}
/**
 */
bool     hwlnSPIClass::dmaSend(const void *data, int size,bool repeat)
{
    if(repeat==false)
        return write(size,(const uint8_t *)data);
    for(int i=0;i<size;i++)
        write(2,(const uint8_t *)data);
    return true;
}
// EOF



