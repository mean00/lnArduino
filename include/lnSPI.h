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
    lnSPISettings(uint32_t speed, spiBitOrder bitOrder, spiDataMode dataMode, int cs)
    {
        this->speed = speed;
        bOrder = bitOrder;
        dMode = dataMode;
        pinCS = cs;
    }

  private:
    int pinCS;          // CS pin associated to the configuration
    uint32_t speed;     // specifies the spi bus maximum clock speed
    spiBitOrder bOrder; // bit order (MSBFirst or LSBFirst)
    spiDataMode dMode;  // one of the data mode

    friend class hwlnSPIClass;
};

typedef void lnSpiCallback(void *cookie);

/**
 *
 * @param instance
 * @param pinCs
 */
class hwlnSPIClass
{
  public:
    hwlnSPIClass(int instance, int pinCs = -1);
    virtual ~hwlnSPIClass();

    void setSSEL(int ssel)
    {
        _internalSettings.pinCS = (ssel);
    };

    void begin();
    void end(void);

    //
    void beginTransaction(lnSPISettings &settings);
    void endTransaction();

    // The settings structure must stay valid while the transaction is on !
    void beginSession(int bitSize);
    void endSession();

    void setBitOrder(spiBitOrder order);
    void setDataMode(spiDataMode mode);
    void setSpeed(int speed); // speed in b/s
    // DMA ones
    void setDataSize(int dataSize); // 8 or 16

    //-- AsyncDma
    // Do asyncDma
    //   and in the callback either call nextDma or finishAsyncDma
    // The caller can call waitForAsyncDmadone to wait till all is done
    bool asyncDmaWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    bool nextDmaWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    bool finishAsyncDma();
    bool waitForAsyncDmaDone();

    // Helper Write functions, make sure you are under a session
    bool write(int z);
    bool write16(int z);
    bool write16Repeat(int nb, const uint16_t pattern);
    bool write(int nbBytes, const uint8_t *data, bool repeat = false);
    bool write16(int nbWord, const uint16_t *data, bool repeat = false);

    bool dmaWrite16(int nbBytes, const uint16_t *data);
    bool dmaWrite16Repeat(int nbBytes, const uint16_t data);
    bool dmaWrite(int nbBytes, const uint8_t *data);

    // slow read/write
    bool transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn);

    // wait for everything to be COMPLETELY done
    void waitForCompletion();
    // This reads over the MOSI pin, i.e. only when only 2 wires are used MOSI + CLK, no MISO
    bool read1wire(int nbRead, uint8_t *rd); // read, reuse MOSI
    //
    int getInstance()
    {
        return _instance;
    }
    int getPeripheralClock();

    lnPin misoPin() const;
    lnPin mosiPin() const;
    lnPin clkPin() const;

  protected:
    void setup();
    void csOn();
    void csOff();

  protected:
    lnSPISettings _internalSettings;
    lnSPISettings _currentSetting;
    lnSPISettings *_settings;
    xMutex _mutex;
    xBinarySemaphore _done;
    bool _useDMA;
    void *_cookie;
    lnSpiCallback *_callback;
    void *_callbackCookie;
    int _instance;
    uint32_t _adr;
    LnIRQ _irq;
    bool _inSession;

    lnDMA txDma;
    // callbacks
    static void exTxDone(void *c, lnDMA::DmaInterruptType it);
    bool writeInternal(int sz, int data);
    bool writesInternal(int sz, int nbBytes, const uint8_t *data, bool repeat = false);
    bool dmaWriteInternal(int wordSize, int nbBytes, const uint8_t *data, bool repeat);

  public:
    void txDone();
    void invokeCallback();
};
// EOF
