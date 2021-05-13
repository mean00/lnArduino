/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnSPI.h"
#include "lnSPI_priv.h"
#include "lnGPIO.h"

struct SpiDescriptor
{
    uint32_t    spiAddress;
    IRQn_Type   spiIrq;
    int         dmaEngine;
    int         dmaTxChannel;
    rcu_periph_enum rcu;
    uint32_t    bank;
    lnPin       mosi,miso, clk;
};

// We assume all the pins of a given SPI are on the same gpio bank
static const SpiDescriptor spiDescriptor[3]=
{
    //             DMA DMATX CLOCK   GPIO MOSI MISO CLK
    {SPI0, SPI0_IRQn,0, 2, RCU_SPI0, GPIOA,PA7, PA6, PA5},
    {SPI1, SPI1_IRQn,0, 4, RCU_SPI1, GPIOB,PB15,PB14,PB13},
    {SPI2, SPI2_IRQn,1, 1, RCU_SPI2, GPIOB,PB5, PB4, PB3}
};
LN_SPI_Registers *aspi0=(LN_SPI_Registers *)SPI0;
/**
 * switch between RX/TX and TX only : false is txRx, 
 * @param adr
 * @param rxTx
 */



void updateMode(LN_SPI_Registers *d,bool rxTx)
{
    
    uint32_t reg=d->CTL0;
    reg&=~(LN_SPI_CTL0_BDOEN | LN_SPI_CTL0_BDEN | LN_SPI_CTL0_RO);
    if(rxTx)
    {
        reg|=0; // 0 all bits = full duplex
    }else
    {   // Tx Only: unidirectional, transmit only, 
        reg|=LN_SPI_CTL0_BDOEN | LN_SPI_CTL0_BDEN;
    }
    d->CTL0=reg;

}
/**
 * 
 * @param adr
 * @param bits
 */
void updateDataSize(LN_SPI_Registers *d,int bits)
{
    
    uint32_t reg=d->CTL0;    
    switch(bits)
    {
        case 8:
             reg&=~LN_SPI_CTL0_FF16;
             break;
        case 16:
            reg|=LN_SPI_CTL0_FF16;
            break;
        default:xAssert(0);
    }
    d->CTL0=reg;
    
}
void updateDmaTX(LN_SPI_Registers *d,bool onoff)
{
    if(onoff)
         d->CTL1 |= (uint32_t)(LN_SPI_CTL1_DMATEN);
    else
         d->CTL1 &= ~((uint32_t)(LN_SPI_CTL1_DMATEN));    
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
    const SpiDescriptor *s=spiDescriptor+instance;
    
    _irq=s->spiIrq;
    eclic_disable_interrupt(_irq);
    _adr=s->spiAddress;
    rcu_periph_clock_enable(s->rcu);
    // setup miso/mosi & clk
    lnPinMode( s->mosi, lnALTERNATE_PP);
    lnPinMode( s->miso, lnFLOATING);
    lnPinMode( s->clk,  lnALTERNATE_PP);
       
    
    if(pinCs!=-1)
    {
        lnPinMode((lnPin)pinCs,lnOUTPUT);
        lnDigitalWrite((lnPin)pinCs,true);
    }
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    _settings=&_internalSettings;
    disable();   
}
/**
 * 
 */
hwlnSPIClass::~hwlnSPIClass()
{
    end();
}
/**
 * 
 */
void hwlnSPIClass::begin()
{   _settings=&_internalSettings; 
    setup();    
}
/**
 * 
 */
void hwlnSPIClass::end(void)
{
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    disable();
}    


void hwlnSPIClass::beginTransaction(lnSPISettings &settings)
{
    _mutex.lock();
    _currentSetting=settings;
    _settings=&_currentSetting;    
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

bool hwlnSPIClass::writeInternal(int sz, int data)
{
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    updateDataSize(d,sz);
    updateMode(d,false);
    enable();
    csOn();
    while (txBusy()) 
    {
    }
    d->DATA=data;
    
    waitForCompletion();
    csOff();
    disable();
    return true;
}

/**
 */
bool hwlnSPIClass::writesInternal(int sz, int nbBytes, const uint8_t *data)
{
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    updateMode(d,false);
    updateDataSize(d,sz);
    enable();
    csOn();
    for (size_t i = 0; i < nbBytes; i++) 
    {
        while (txBusy()) 
        {
        }
        d->DATA=data[i];
    }
    waitForCompletion();
    csOff();
    disable();
    return true;
}
/**
 * 
 * @param z
 * @return 
 */
bool hwlnSPIClass::write(int z)
{
    return writeInternal(8,z);
}
/**
 * 
 * @param z
 * @return 
 */
bool hwlnSPIClass::write16(int z)
{
    return writeInternal(16,z);
}

/**
 */
bool hwlnSPIClass::write(int nbBytes, const uint8_t *data)
{
   return writesInternal(8,nbBytes,data);
}
/**
 * 
 */
void hwlnSPIClass::csOn()
{
    if(_settings->pinCS!=-1)
    {
        lnDigitalWrite((lnPin)_settings->pinCS,false);
    }
}
/**
 * 
 */
void hwlnSPIClass::csOff()
{
    if(_settings->pinCS!=-1)
    {
        lnDigitalWrite((lnPin)_settings->pinCS,true);
    }
}
/**
 *  This is needed to be able to toggle the CS when all is done
 */
void hwlnSPIClass::waitForCompletion()
{
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    int dir=d->CTL0>>14;
    switch(dir)
    {
        case 0:
        case 1: // bidir mode
            while(busy())
                    {

                    }
            break;
        case 2: // receive only
            xAssert(0);
            break;
        case 3: //  t only
            while(txBusy())
            {
                
            }
            break;
    }
}
/**
 */
bool hwlnSPIClass::transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn)
{
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    updateMode(d,true);
    updateDataSize(d,8);
    enable();
    csOn();
    for (size_t i = 0; i < nbBytes; i++) 
    {
        while (!d->STAT & LN_SPI_STAT_TBE) 
        {
        }
        d->DATA=dataOut[i];
        
        while (!(d->STAT & LN_SPI_STAT_RBNE) )
        {        
        }
        dataIn[i] = d->DATA;
    }
    waitForCompletion();
    csOff();
    disable();
    return true;
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
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    xAssert(_settings);
    disable();
    d->CTL0&=LN_SPI_CTL0_MASK;
    d->CTL1&=LN_SPI_CTL1_MASK;
    d->STAT&=LN_SPI_STAT_MASK;
    
    d->CTL0|=LN_SPI_CTL0_MSTMODE;
    
    switch(_settings->bOrder)
    {
        case   SPI_LSBFIRST: d->CTL0|=LN_SPI_CTL0_LSB;break;
        case   SPI_MSBFIRST: break;
        default:xAssert(0);
                break;            
    }
    uint32_t s=0;
    switch(_settings->dMode)
    {
        case SPI_MODE0:
            s=0; // Low 1 edge
            break;
        case SPI_MODE1:
            s=LN_SPI_CTL0_CKPH; // Low 2 edge
            break;
        case SPI_MODE2:
            s=LN_SPI_CTL0_CKPL; // high , 1 edge
            break;
        case SPI_MODE3:
            s=LN_SPI_CTL0_CKPL|LN_SPI_CTL0_CKPH; // high , 2 edge
            break;
        default:xAssert(0);
                break;
    }     
    d->CTL0|=s;
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
    prescale = (apb+speed/2) / speed;
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
    d->CTL0|=psc<<3;
    updateMode(0,false); // Tx only by default    
}
/**
 * 
 * @param dataSize
 */
void hwlnSPIClass::setDataSize(int dataSize)
{
    LN_SPI_Registers *d=(LN_SPI_Registers *)_adr;
    updateDataSize(d,dataSize);
}

void hwlnSPIClass::setBitOrder(spiBitOrder order)
{
       _internalSettings.bOrder=order; 
       setup();
}
void hwlnSPIClass::setDataMode(spiDataMode mode)
{
      _internalSettings.dMode=mode;
      setup();
}
void hwlnSPIClass::setSpeed(int speed)
{
    _internalSettings.speed=speed;
    setup();
}
// EOF



