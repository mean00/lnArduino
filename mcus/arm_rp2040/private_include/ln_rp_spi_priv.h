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
#include "ln_rp_dma.h"
/**
 */
typedef struct
{
    uint32_t CR0;   // 0x00
    uint32_t CR1;   // 0x04
    uint32_t DR;    // 0x08  data
    uint32_t SR;    // 0x0C  status register
    uint32_t CPSR;  // 0x10  clock prescaler
    uint32_t IMSC;  // 0x14  interrupt mask
    uint32_t RIS;   // 0x18  raw interrup status
    uint32_t MIS;   // 0x1C  masked status
    uint32_t ICR;   // 0x20  interrupt clear
    uint32_t DMACR; // 0x24
} LN_RP_SPIx;

typedef volatile LN_RP_SPIx LN_RP_SPI;

#define LN_RP_SPI_CR0_DIVIDER(x) (x << 8)
#define LN_RP_SPI_CR0_DIVIDER_MASK (0xf << 8)
#define LN_RP_SPI_CR0_MODE(x) (x << 6) // PHASE  / POLARITY, motoroal only
#define LN_RP_SPI_CR0_FORMAT_MOTOROLA (0 << 4)
#define LN_RP_SPI_CR0_FORMAT_TI (1 << 4)
#define LN_RP_SPI_CR0_FORMAT_NL (2 << 4)
#define LN_RP_SPI_CR0_8_BITS (7 << 0)
#define LN_RP_SPI_CR0_16_BITS (0xf << 0)
//
#define LN_RP_SPI_CR1_ENABLE (1 << 1)
// Status
#define LN_RP_SPI_SR_TFE (1 << 0) // Tx empty
#define LN_RP_SPI_SR_TFN (1 << 1) // tx not full
#define LN_RP_SPI_SR_RNE (1 << 2) // rx not empty
#define LN_RP_SPI_SR_RFF (1 << 3) // rx full
#define LN_RP_SPI_SR_BSY (1 << 4) // busy

// interrupt mask set
#define LN_RP_SPI_INT_OVR (1 << 0) // overrun RORIM
#define LN_RP_SPI_INT_TIM (1 << 1) // timeout RTIM
#define LN_RP_SPI_INT_RX (1 << 2)  // receive
#define LN_RP_SPI_INT_TX (1 << 3)  // transmit

// interrupt raw status

// Masked interrup tstatus status
// both same as mask set

// Interrupt clear register
#define LN_RP_SPI_ICR_OVR (1 << 0) // overrun
#define LN_RP_SPI_ICR_TIM (1 << 1) // timeout

// DMA
#define LN_RP_SPI_DMACR_RX (1 << 0) //
#define LN_RP_SPI_DMACR_TX (1 << 1) //

//----
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

    // bool    dmaWrite16(int nbBytes, const uint16_t *data);
    // bool    dmaWrite8(int nbBytes, const uint8_t *data);
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
    lnRpDMA _txDma;
};
