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

/**
 * 
 * @param instance
 * @param pinCs
 */
hwlnSPIClass::hwlnSPIClass(int instance, int pinCs) : _internalSettings(1000000,SPI_MSBFIRST,SPI_MODE0,-1)
{
    _useDMA=false;
    _cookie=NULL;
    _callback=NULL;
    _instance=instance;
}

/**
 * 
 */
void hwlnSPIClass::begin()
{

    _currentSetting=&_internalSettings;

}
/**
 * 
 */
void hwlnSPIClass::end(void)
{
}    


void hwlnSPIClass::beginTransaction(lnSPISettings settings)
{
    _currentSetting=&settings;
}
/**
 */
void hwlnSPIClass::endTransaction()
{
    _currentSetting=&_internalSettings;
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

#if 0   
  private:
    lnSPISettings       _internalSettings;
    lnSPISettings       *_currentSetting;
    xMutex              _mutex;
    xBinarySemaphore    _done;
    bool                _useDMA;
};
#endif
// EOF



