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

extern "C"
{
    #include "gd32vf103_spi.h"
}

/**
 * 
 * @param instance
 * @param pinCs
 */
hwlnSPIClass::hwlnSPIClass(int instance, int pinCs) : _internalSettings(1000000,SPI_MSBFIRST,SPI_MODE0,-1),_currentSetting(1000000,SPI_MSBFIRST,SPI_MODE0,-1)
{
    _useDMA=false;
    _cookie=NULL;
    _callback=NULL;
    _instance=instance;
    _settings=NULL;
    switch(instance)
    {
        case 0:
                _adr=SPI0;
                _irq=SPI0_IRQn;
                rcu_periph_clock_enable(RCU_SPI0);
                break;
        case 1:
                _adr=SPI1;
                _irq=SPI1_IRQn;
                rcu_periph_clock_enable(RCU_SPI1);
                break;
        case 2:
                _adr=SPI2;
                _irq=SPI2_IRQn;
                rcu_periph_clock_enable(RCU_SPI2);
                break;
        default:
            xAssert(0);
            break;
    }
    
    spi_i2s_deinit(_adr);
}

/**
 * 
 */
void hwlnSPIClass::begin()
{    
    _settings=&_internalSettings;
}
/**
 * 
 */
void hwlnSPIClass::end(void)
{
}    


void hwlnSPIClass::beginTransaction(lnSPISettings &settings)
{
    _mutex.lock();
    _currentSetting=settings;
    _settings=&settings;
    
    // setup
    
}
/**
 */
void hwlnSPIClass::endTransaction()
{
    _settings=&_internalSettings;
    _mutex.unlock();
}
/**
 * 
 * @param nbBytes
 * @param data
 * @param callback
 * @return 
 */
bool hwlnSPIClass::asyncWrite(int nbBytes, uint8_t *data,lnSpiCallback *callback,void *cookie)
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
 */
bool hwlnSPIClass::write(int nbBytes, uint8_t *data)
{
    xAssert(0);
    return false;
}
/**
 */
bool hwlnSPIClass::transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn)
{
    xAssert(0);
    return false;

}
/**
 * 
 */
void hwlnSPIClass::setup()
{
    
}
// EOF



