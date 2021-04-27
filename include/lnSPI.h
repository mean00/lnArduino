#pragma once
#include "lnArduino.h"
extern "C"
{
#include "gd32vf103_spi.h"
}

//Mode          Clock Polarity (CPOL)   Clock Phase (CPHA)
//SPI_MODE0             0                     0
//SPI_MODE1             0                     1
//SPI_MODE2             1                     0
//SPI_MODE3             1                     1
enum spiDataMode
{
    SPI_MODE0=0,
    SPI_MODE1=1,
    SPI_MODE2=2,
    SPI_MODE3=3
};
/**
 */
enum spiBitOrder
{
    SPI_LSBFIRST=0,
    SPI_MSBFIRST=1
};
/**
 */
class lnSPISettings 
{
  public:
    lnSPISettings(uint32_t speed, spiBitOrder bitOrder, spiDataMode dataMode,int cs)
    {
      this->speed= speed;
      bOrder    = bitOrder;
      dMode     = dataMode;
      pinCS     = cs;
    }   
  private:
    int         pinCS;      //CS pin associated to the configuration
    uint32_t    speed;       //specifies the spi bus maximum clock speed
    spiBitOrder bOrder;    //bit order (MSBFirst or LSBFirst)
    spiDataMode dMode;   //one of the data mode
    
    friend class hwlnSPIClass;
};
/**
 * 
 * @param instance
 * @param pinCs
 */
class hwlnSPIClass 
{
  public:
    hwlnSPIClass(int instance, int pinCs=-1);
    
    void setSSEL(int ssel)
    {
      _internalSettings.pinCS = (ssel);
    };

    void begin();
    void end(void);

    /* This function should be used to configure the SPI instance in case you
     * don't use default parameters.
     * You can attach another CS pin to the SPI instance and each CS pin can be
     * attach with specific SPI settings.
     */
    void beginTransaction(lnSPISettings settings);
    void endTransaction();

     void setBitOrder(spiBitOrder order)
    {
         _internalSettings.bOrder=order;      
    }
    void setDataMode(spiDataMode mode)
    {
        _internalSettings.dMode=mode;
    }
    void setSspeed(int speed)
    {
      _internalSettings.speed=speed;
    }
    
   
    bool write(int nbBytes, uint8_t *data);
    bool transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn);
    
   
  private:
    lnSPISettings       _internalSettings;
    lnSPISettings       *_currentSetting;
    xMutex              _mutex;
    xBinarySemaphore    _done;
    bool                _useDMA;
};
// EOF



