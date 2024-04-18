/**
 * @file ln_rp_spi_priv.h
 * @author mean00
 * @brief
 * @version 0.1
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "lnArduino.h"
#include "lnSPI.h"
class lnRpDMA;
class LN_RP_SPIx;
typedef volatile LN_RP_SPIx LN_RP_SPI;
/**
 *
 * @param instance
 * @param pinCs
 */
class rpSPI : public lnSPI
{
  public:
    enum spiTxState
    {
        TxStateBody,
        TxStateLast
    };

  public:
            rpSPI(int instance, int pinCs = -1);
    virtual ~rpSPI();


    virtual void setBitOrder(spiBitOrder order);
    virtual void setDataMode(spiDataMode mode);

    void begin(int wordsize = 8);
    void begin();
    void end(void);

    void setSpeed(int speed);       // speed in b/s
    void setDataSize(int dataSize); // 8 or 16

    bool write8(int nbBytes, const uint8_t *data);
    bool write16(int nbHalfWord, const uint16_t *data);
    bool write16(int nbHalfWord, const uint16_t *data, bool repeat);
    bool write(int nbBytes, const uint8_t *data, bool repeat);
    bool write(int z);
    bool write16(int z);
    bool write16Repeat(int nb, const uint16_t pattern);

//-
    bool dmaWrite8(int nbBytes, const uint8_t *data);
    bool dmaWrite8Repeat(int nbBytes, const uint8_t data);
    

//

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
    virtual void beginTransaction(lnSPISettings &settings);
    virtual void endTransaction();

    // The settings structure must stay valid while the transaction is on !
    virtual void beginSession(int bitSize);
    virtual void endSession();

    lnPin misoPin() const;
    lnPin mosiPin() const;
    lnPin clkPin() const;
    uint32_t getPeripheralClock();
    
protected:
    //-- AsyncDma
    // Do asyncDma
    //   and in the callback either call nextDma or finishAsyncDma
    // The caller can call waitForAsyncDmadone to wait till all is done
    virtual bool asyncDmaWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    virtual bool nextDmaWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat = false);
    virtual bool finishAsyncDma();
    virtual bool waitForAsyncDmaDone();



//-


  public:
    void irqHandler();
    void dmaHandler();

  protected:
    uint32_t    _cr0, _cr1, _prescaler;
    uint32_t    _instance;
    int         _wordSize; // 8 or 16
    lnPin       _cs;
    LN_RP_SPI   *_spi;
    const uint8_t *_current, *_limit;

    lnBinarySemaphore _txDone;
    spiTxState  _state;
    lnRpDMA     *_txDma;
};
