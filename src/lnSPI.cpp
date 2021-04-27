#include "lnArduino.h"
#include "lnSPI.h"

/**
 * 
 * @param instance
 * @param pinCs
 */
hwlnSPIClass::hwlnSPIClass(int instance, int pinCs) : _internalSettings(1000000,SPI_MSBFIRST,SPI_MODE0,-1)
{

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

/* This function should be used to configure the SPI instance in case you
 * don't use default parameters.
 * You can attach another CS pin to the SPI instance and each CS pin can be
 * attach with specific SPI settings.
 */
void hwlnSPIClass::beginTransaction(lnSPISettings settings)
{
}
/**
 */
void endTransaction()
{

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



