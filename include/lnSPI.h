/*
 *  (C) 2021/2022 MEAN00 fixounet@free.fr
 *  See license file
 *
 * Normal usage :
 * --------------
 *    setup()
 *    begin()
 *    ...
 *    ...
 *    beginTransaction() <= Lock on a specific device till endTransaction
 *    beginSession(8)  <= while in beginSession the SPI stays enabled and the CS stays on (if defined),
 *                             also the bus width stays the same (8/16 bits)
 *    write8
 *    write8
 *    read8
 *    endSession()
 *    beginSession(16)
 *    write16
 *    write16
 *    read16
 *    endSession()
 * endTransaction() <= Other devices can be used
 *
 *
 *
 */

#pragma once
#include "lnArduino.h"
#include "lnDma.h"

// Mode          Clock Polarity (CPOL)   Clock Phase (CPHA)
// SPI_MODE0             0                     0
// SPI_MODE1             0                     1
// SPI_MODE2             1                     0
// SPI_MODE3             1                     1
enum spiDataMode
{
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3
};
/**
 */
enum spiBitOrder
{
    SPI_LSBFIRST = 0,
    SPI_MSBFIRST = 1
};
/**
 * This is not really used
 */
class lnSPISettings
{
  public:
    lnSPISettings(uint32_t ispeed, spiBitOrder bitOrder, spiDataMode dataMode, int cs)
    {
        speed = ispeed;
        bOrder = bitOrder;
        dMode = dataMode;
        pinCS = cs;
    }

    int pinCS;          // CS pin associated to the configuration
    uint32_t speed;     // specifies the spi bus maximum clock speed
    spiBitOrder bOrder; // bit order (MSBFirst or LSBFirst)
    spiDataMode dMode;  // one of the data mode
};

typedef void lnSpiCallback(void *cookie);

/**
 *
 * @param instance
 * @param pinCs
 */
class lnSPI
{
  public:
    static lnSPI *create(int instance, int pinCs = -1);
  
    virtual ~lnSPI() {}

    

    virtual void begin()=0;    
    void set(const lnSPISettings &st)
    {
        setSSEL(st.pinCS);
        setSpeed(st.speed);
        setBitOrder(st.bOrder);
        setDataMode(st.dMode);
    }
    virtual void setBitOrder(spiBitOrder order)=0;
    virtual void setDataMode(spiDataMode mode)=0;
    virtual void setSpeed(int speed)=0; // speed in b/s
            void setSSEL(int ssel)    {        _internalSettings.pinCS = (ssel);    };
    // async API
    virtual bool asyncWrite8(int nbBytes, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false)=0;
    virtual bool nextWrite8(int nbBytes, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false)=0;
    virtual bool asyncWrite16(int nbWords, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false)=0;
    virtual bool nextWrite16(int nbWords, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false)=0;
    virtual bool finishAsyncDma()=0;
    virtual bool waitForAsync()=0;

    // Helper Write functions, make sure you are under a session
    virtual bool write8(uint8_t z)=0;
    virtual bool write16(uint16_t z)=0;

    virtual bool write16(int nbWord, const uint16_t *data)=0;
    virtual bool write16Repeat(int nbWord, const uint16_t data)=0;
    virtual bool write8(int nbBytes, const uint8_t *data)=0;
    virtual bool write8Repeat(int nbBytes, const uint8_t data)=0;
    virtual void waitForCompletion() const=0;
    // slow read/write
    virtual bool transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn)=0;

    // wait for everything to be COMPLETELY done
    //virtual void waitForCompletion()=0;
    // This reads over the MOSI pin, i.e. only when only 2 wires are used MOSI + CLK, no MISO
    virtual bool read1wire(int nbRead, uint8_t *rd)=0; // read, reuse MOSI
    //
    int getInstance()
    {
        return _instance;
    }    
    
protected:
  int             _instance;    
  lnSPISettings   _internalSettings;  
  lnSpiCallback   *_callback;
  void            *_callbackCookie;

protected:
    lnSPI(int instance, int pinCs = -1) :
     _internalSettings(1000000, SPI_MSBFIRST, SPI_MODE0, -1),      
     _callback(NULL), _callbackCookie(NULL)
      {
      }
};
// EOF
