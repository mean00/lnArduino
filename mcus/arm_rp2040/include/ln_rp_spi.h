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

class lnRpDMA;
class LN_RP_SPIx;
typedef volatile LN_RP_SPIx LN_RP_SPI;
/**
 *
 * @param instance
 * @param pinCs
 */
class rpSPI
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
    void begin();
    void end(void);

    // void    setBitOrder(spiBitOrder order);
    // void    setDataMode(spiDataMode mode);
    void setSpeed(int speed);       // speed in b/s
    void setDataSize(int dataSize); // 8 or 16

    bool write8(int nbBytes, const uint8_t *data);

  public:
    void irqHandler();
    void dmaHandler();

  protected:
    uint32_t _cr0, _cr1, _prescaler;
    uint32_t _instance;
    lnPin _cs;
    LN_RP_SPI *_spi;
    const uint8_t *_current, *_limit;

    lnBinarySemaphore _txDone;
    spiTxState _state;
    lnRpDMA *_txDma;
};
