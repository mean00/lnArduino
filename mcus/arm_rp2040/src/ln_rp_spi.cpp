
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
#include "ln_rp_memory_map.h"
#include "ln_rp_spi_priv.h"

class rpSPI;

typedef struct
{
    LN_RP_SPI *spi;
    LnIRQ irq;
} SPI_DESC;

const SPI_DESC spis[2] = {
    {(LN_RP_SPI *)LN_RP_SPI0, (LnIRQ)18},
    {(LN_RP_SPI *)LN_RP_SPI1, (LnIRQ)19},
};

typedef void lnSpiCallback(void *cookie);
static rpSPI *instances[2] = {NULL, NULL};

static void spiHandler(int x)
{
    rpSPI *s = instances[x];
    xAssert(s);
    s->irqHandler();
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
rpSPI::rpSPI(int instance, int pinCs)
{
    xAssert(!instances[instance]);
    instances[instance] = this;
    _cs = (lnPin)pinCs;
    _instance = instance;
    _spi = (LN_RP_SPI *)spis[instance].spi;
    _cr0 = LN_RP_SPI_CR0_DIVIDER(100) + LN_RP_SPI_CR0_MODE(0) + LN_RP_SPI_CR0_FORMAT_MOTOROLA + LN_RP_SPI_CR0_8_BITS;
    _cr1 = 0;
    _scaler = 100;
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
    _spi->CPSR = _scaler;
    _spi->IMSC = 0;
    _spi->CR1 |= LN_RP_SPI_CR1_ENABLE;
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
    lnEnableInterrupt(spis[_instance].irq);
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

//--