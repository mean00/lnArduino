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

    void setSSEL(int ssel)
    {
        _internalSettings.pinCS = (ssel);
    };

    virtual void begin()=0;
    virtual void end(void)=0;
  

    //
    virtual void beginTransaction(lnSPISettings &settings)=0;
    virtual void endTransaction()=0;

    // The settings structure must stay valid while the transaction is on !
    virtual void beginSession(int bitSize)=0;
    virtual void endSession()=0;

    virtual void setBitOrder(spiBitOrder order)=0;
    virtual void setDataMode(spiDataMode mode)=0;
    virtual void setSpeed(int speed)=0; // speed in b/s
    // DMA ones
    virtual void setDataSize(int dataSize)=0; // 8 or 16

    //-- AsyncDma
    // Do asyncDma
    //   and in the callback either call nextDma or finishAsyncDma
    // The caller can call waitForAsyncDmadone to wait till all is done
    virtual bool asyncDmaWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false)=0;
    virtual bool nextDmaWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false)=0;
    virtual bool finishAsyncDma()=0;
    virtual bool waitForAsyncDmaDone()=0;

    // Helper Write functions, make sure you are under a session
    virtual bool write(int z)=0;
    virtual bool write16(int z)=0;
    virtual bool write16Repeat(int nb, const uint16_t pattern)=0;
    virtual bool write(int nbBytes, const uint8_t *data, bool repeat = false)=0;
    virtual bool write16(int nbWord, const uint16_t *data, bool repeat = false)=0;

    virtual bool dmaWrite16(int nbBytes, const uint16_t *data)=0;
    virtual bool dmaWrite16Repeat(int nbBytes, const uint16_t data)=0;
    virtual bool dmaWrite(int nbBytes, const uint8_t *data)=0;

    // slow read/write
    virtual bool transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn)=0;

    // wait for everything to be COMPLETELY done
    virtual void waitForCompletion()=0;
    // This reads over the MOSI pin, i.e. only when only 2 wires are used MOSI + CLK, no MISO
    virtual bool read1wire(int nbRead, uint8_t *rd)=0; // read, reuse MOSI
    //
    int getInstance()
    {
        return _instance;
    }
    virtual uint32_t getPeripheralClock()=0;

    virtual lnPin misoPin() const=0;
    virtual lnPin mosiPin() const=0;
    virtual lnPin clkPin() const=0;
protected:
  int             _instance;    
  lnSPISettings   _internalSettings;
  lnSPISettings   _currentSetting;
  lnSpiCallback   *_callback;
  void            *_callbackCookie;

protected:
    lnSPI(int instance, int pinCs = -1) :
     _internalSettings(1000000, SPI_MSBFIRST, SPI_MODE0, -1), 
     _currentSetting(1000000, SPI_MSBFIRST, SPI_MODE0, -1),
     _callback(NULL), _callbackCookie(NULL)
      {
      }


};
// EOF
