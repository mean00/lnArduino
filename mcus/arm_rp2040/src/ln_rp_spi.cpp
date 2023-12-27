
/**
 * @file ln_rp_spi.cpp
 * @author mean00
 * @brief
 * @version 0.1
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "lnArduino.h"
#include "lnGPIO.h"
#include "lnSPI.h"
#include "ln_rp_dma.h"
#include "ln_rp_memory_map.h"
#include "ln_rp_spi_priv.h"
extern "C"
{
#include "hardware/structs/clocks.h"
    uint32_t clock_get_hz(enum clock_index clk_index);
}
class rpSPI;
#define RP_SPI_USE_DMA 1
typedef struct
{
    LN_RP_SPI *spi;
    LnIRQ irq;
    lnRpDMA::LN_RP_DMA_DREQ txDma;
} SPI_DESC;

const SPI_DESC spis[2] = {
    {(LN_RP_SPI *)LN_RP_SPI0, (LnIRQ)18, lnRpDMA::LN_DMA_DREQ_SPI0_TX},
    {(LN_RP_SPI *)LN_RP_SPI1, (LnIRQ)19, lnRpDMA::LN_DMA_DREQ_SPI1_TX},
};

typedef void lnSpiCallback(void *cookie);
static rpSPI *instances[2] = {NULL, NULL};

static void spiHandler(int x)
{
    rpSPI *s = instances[x];
    xAssert(s);
    s->irqHandler();
}
static void dmaCb(void *cookie)
{
    rpSPI *spi = (rpSPI *)cookie;
    spi->dmaHandler();
}
static void spiHandler0()
{
    spiHandler(0);
}
static void spiHandler1()
{
    spiHandler(1);
}

/**
 * @brief Construct a new rp S P I::rp S P I object
 *
 * @param instance
 * @param pinCs
 */
rpSPI::rpSPI(int instance, int pinCs) : _txDma(lnRpDMA::DMA_MEMORY_TO_PERIPH, spis[instance].txDma, 8)
{
    xAssert(!instances[instance]);
    instances[instance] = this;
    _cs = (lnPin)pinCs;
    _instance = instance;
    _spi = (LN_RP_SPI *)spis[instance].spi;
    _cr0 = LN_RP_SPI_CR0_DIVIDER(25) + LN_RP_SPI_CR0_MODE(0) + LN_RP_SPI_CR0_FORMAT_MOTOROLA + LN_RP_SPI_CR0_8_BITS;
    _cr1 = 0;
    _prescaler = 250;
    _txDma.attachCallback(dmaCb, this);
    if (!_instance)
        lnSetInterruptHandler(spis[instance].irq, spiHandler0);
    else
        lnSetInterruptHandler(spis[instance].irq, spiHandler1);
}
/**
 * @brief Destroy the rp S P I::rp S P I object
 *
 */
rpSPI::~rpSPI()
{
    end();
    instances[_instance] = NULL;
}
/**
 * @brief
 *
 */
void rpSPI::begin()
{
    _spi->CR0 = _cr0;
    _spi->CR1 = _cr1;
    _spi->CPSR = _prescaler;
    _spi->IMSC = 0;
    _spi->DMACR = LN_RP_SPI_DMACR_RX |
                  LN_RP_SPI_DMACR_TX; // enable DMA, activating the correct DMA *or* IRQ will select DMA or not

    _spi->CR1 |= LN_RP_SPI_CR1_ENABLE;
    _txDma.doMemoryToPeripheralTransferNoLock(1, (const uint32_t *)NULL, (const uint32_t *)&(_spi->DR));
    _txDma.armTransfer();
}
/**
 * @brief
 *
 */
void rpSPI::end(void)
{
    _spi->CR1 &= ~LN_RP_SPI_CR1_ENABLE;
    _spi->DMACR = 0;
    _spi->IMSC = 0;
}
/**
 * @brief
 *
 * @param speed
 */
void rpSPI::setSpeed(int speed)
{

    uint32_t fq_in = clock_get_hz(clk_peri);

#define RATIO 128

    int div = (fq_in) / (speed);
    int org = (div / RATIO);
    _prescaler = org & ~1;
    if (!_prescaler)
        _prescaler = 2;
    int scaler = ((div + (_prescaler >> 1)) / _prescaler) - 1;
    _cr0 &= ~LN_RP_SPI_CR0_DIVIDER_MASK;
    _cr0 |= LN_RP_SPI_CR0_DIVIDER(scaler);
}

/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @return true
 * @return false
 */
bool rpSPI::write8(int nbBytes, const uint8_t *data)
{
    _current = data;
    _limit = _current + nbBytes;
    _spi->IMSC |= LN_RP_SPI_INT_TX;
    _state = TxStateBody;
    _txDone.tryTake();
#ifdef RP_SPI_USE_DMA
    _txDma.continueMemoryToPeripheralTransferNoLock(nbBytes, (const uint32_t *)data);
#else
    lnEnableInterrupt(spis[_instance].irq);
#endif
    _txDone.take();
    return true;
}
/**
 * @brief
 *
 */
void rpSPI::irqHandler()
{
    switch (_state)
    {
    case TxStateBody: {
        while (_current < _limit)
        {
            if (_spi->SR & LN_RP_SPI_SR_TFN)
            {
                _spi->DR = *_current;
                _current++;
            }
            else
            {
                break;
            }
        }
        if (_current == _limit)
        {
            _state = TxStateLast;
            _spi->IMSC = 0;
            lnDisableInterrupt(spis[_instance].irq);
            _txDone.give();
        }
    }
    break;
    default:
        xAssert(0);
        break;
    }
}
/**
 * @brief
 *
 */
void rpSPI::dmaHandler()
{
    _spi->IMSC = 0;
    _txDone.give();
}
//--