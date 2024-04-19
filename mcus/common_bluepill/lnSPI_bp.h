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
#include "lnSPI.h"

/**
 *
 * @param instance
 * @param pinCs
 */
class lnSPI_bp : public lnSPI
{
  public:
    lnSPI_bp(int instance, int pinCs = -1);
    virtual ~lnSPI_bp();

    virtual void begin();    
    virtual void setBitOrder(spiBitOrder order);
    virtual void setDataMode(spiDataMode mode);
    virtual void setSpeed(int speed); // speed in b/s
            void setSSEL(int ssel)    {        _internalSettings.pinCS = (ssel);    };
    // async API
    virtual bool asyncWrite8(int nbBytes, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    virtual bool nextWrite8(int nbBytes, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    virtual bool asyncWrite16(int nbWords, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    virtual bool nextWrite16(int nbWords, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    virtual bool finishAsyncDma();
    virtual bool waitForAsync();

    // Helper Write functions, make sure you are under a session
    virtual bool write8(const uint8_t z);
    virtual bool write16(const uint16_t z);

    virtual bool write8(int nbBytes, const uint8_t *data);
    virtual bool write8Repeat(int nbBytes, const uint8_t data);

    virtual bool write16(int nbBytes, const uint16_t *data);
    virtual bool write16Repeat(int nbBytes, const uint16_t data);
    virtual void waitForCompletion() const;
    // slow read/write
    virtual bool transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn);

    // wait for everything to be COMPLETELY done
    //virtual void waitForCompletion()=0;
    // This reads over the MOSI pin, i.e. only when only 2 wires are used MOSI + CLK, no MISO
    virtual bool read1wire(int nbRead, uint8_t *rd); // read, reuse MOSI
    //

  protected:
    void setup();
    void csOn();
    void csOff();

  protected:
    
    xBinarySemaphore _done;        
    
    uint32_t _adr;
    LnIRQ _irq;    

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
