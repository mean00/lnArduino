
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
#include "ln_rp_spi.h"
#include "lnArduino.h"
#include "lnGPIO.h"
#include "lnSPI.h"
#include "ln_rp_dma.h"
#include "ln_rp_memory_map.h"
#include "ln_rp_spi_priv.h"

// #define NO_DMA 1

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
rpSPI::rpSPI(int instance, int pinCs) : lnSPI(instance, pinCs)
{
    xAssert(!instances[instance]);
    instances[instance] = this;
    _cs = (lnPin)pinCs;
    _instance = instance;
    _spi = (LN_RP_SPI *)spis[instance].spi;
    _cr0 = LN_RP_SPI_CR0_DIVIDER(25) + LN_RP_SPI_CR0_MODE(0) + LN_RP_SPI_CR0_FORMAT_MOTOROLA;
    _cr1 = 0;
    _prescaler = 250;
    _wordSize = 8;

    _txDma = new lnRpDMA(lnRpDMA::DMA_MEMORY_TO_PERIPH, spis[_instance].txDma, _wordSize);
    _txDma->attachCallback(dmaCb, this);
    if (!_instance)
        lnSetInterruptHandler(spis[_instance].irq, spiHandler0);
    else
        lnSetInterruptHandler(spis[_instance].irq, spiHandler1);
    _txDma->doMemoryToPeripheralTransferNoLock(1, (const uint32_t *)NULL, (const uint32_t *)&(_spi->DR));
}
/**
 * @brief Destroy the rp S P I::rp S P I object
 *
 */
rpSPI::~rpSPI()
{
    end();
    delete _txDma;
    _txDma = NULL;
    instances[_instance] = NULL;
}
/**
 * @brief
 *
 * @param wordsize
 */
void rpSPI::begin(int wordsize)
{
    _wordSize = wordsize;
    xAssert(_wordSize == 8 || _wordSize == 16);
    _cr0 &= ~LN_RP_SPI_CR0_BITS_MASK;
    switch (_wordSize)
    {
    case 8:
        _cr0 |= LN_RP_SPI_CR0_8_BITS;
        break;
    case 16:
        _cr0 |= LN_RP_SPI_CR0_16_BITS;
        break;
    default:
        xAssert(0);
        break;
    }

    _spi->CR0 = _cr0;
    _spi->CR1 = _cr1;
    _spi->CPSR = _prescaler;
    _spi->IMSC = 0;
    _spi->DMACR = 0; /*LN_RP_SPI_DMACR_RX |  LN_RP_SPI_DMACR_TX */
    ;                // enable DMA, activating the correct DMA *or* IRQ will select DMA or not

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
 * @param order
 */
void rpSPI::setBitOrder(spiBitOrder order)
{
    switch (order)
    {
    case SPI_LSBFIRST:
        xAssert(0);
        break;
    case SPI_MSBFIRST:
        break;
    default:
        xAssert(0);
        break;
    }
}

/**
 * @brief
 *
 * @param dataSize
 */
void rpSPI::setDataSize(int dataSize)
{
    _cr0 &= ~LN_RP_SPI_CR0_BITS_MASK;
    switch (dataSize)
    {
    case 8:
        _cr0 |= LN_RP_SPI_CR0_8_BITS;
        break;
    case 16:
        _cr0 |= LN_RP_SPI_CR0_16_BITS;
        break;
    default:
        xAssert(0);
        break;
    }
    _spi->CR0 = _cr0;
}

/**
 * @brief
 *
 * @param mode
 */
void rpSPI::setDataMode(spiDataMode mode)
{
    _cr0 &= ~LN_RP_SPI_CR0_MODE(3);
    uint32_t r = 0;
    switch (mode)
    {
    case SPI_MODE0:
        r = 0;
        break;
    case SPI_MODE1:
        r = 2;
        break;
    case SPI_MODE2:
        r = 1;
        break;
    case SPI_MODE3:
        r = 3;
        break;
    default:
        xAssert(0);
        break;
    }
    _cr0 |= LN_RP_SPI_CR0_MODE(r);
    _spi->CR0 = _cr0;
}
/**
 * @brief
 *
 */
void rpSPI::waitForCompletion() const
{
    int countdown = 10 * 1000 * 1000; // 100M/90k =>
    while ((_spi->SR & LN_RP_SPI_SR_BSY))
    {
        __asm__("nop");
        if (--countdown == 0)
        {
            Logger("SPI Timeout !\n");
            return;
        }
    }
}
/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @return true
 * @return false
 */
bool rpSPI::blockWrite8(int nbBytes, const uint8_t *data)
{
    xAssert(_wordSize == 8);
    _callback = nullptr;
    _current = data;
    _limit = _current + nbBytes;
    _spi->DMACR = LN_RP_SPI_DMACR_RX | LN_RP_SPI_DMACR_TX;
    _spi->IMSC |= LN_RP_SPI_INT_TX;
    _state = TxStateBody;
    _txDone.tryTake();
    _txDma->setTransferSize(8);
    _txDma->doMemoryToPeripheralTransferNoLock(nbBytes, (const uint32_t *)data, (const uint32_t *)&(_spi->DR), false);
    _txDma->beginTransfer();
    _txDone.take();
    waitForCompletion();
    return true;
}
/**
 * @brief
 *
 * @param nbHalfWord
 * @param data
 * @return true
 * @return false
 */
bool rpSPI::blockWrite16(int nbHalfWord, const uint16_t *data)
{
    xAssert(_wordSize == 16);
    _callback = nullptr;
    _current = (const uint8_t *)data;
    _limit = _current + nbHalfWord * 2;
    _spi->IMSC |= LN_RP_SPI_INT_TX;
    _spi->DMACR = LN_RP_SPI_DMACR_RX | LN_RP_SPI_DMACR_TX;
    _state = TxStateBody;
    _txDone.tryTake();
    _txDma->setTransferSize(16);
    _txDma->doMemoryToPeripheralTransferNoLock(nbHalfWord, (const uint32_t *)data, (const uint32_t *)&(_spi->DR),
                                               false);
    _txDone.take();
    waitForCompletion();
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
    if (_callback)
    {
        _callback(_callbackCookie);
    }
    else
    {
        _txDma->endTransfer();
        _txDone.give();
    }
}

/**
 * @brief
 *
 * @param settings
 */
void rpSPI::set(lnSPISettings &settings)
{
    setBitOrder(settings.bOrder);
    setDataMode(settings.dMode);
    setSpeed((int)settings.speed);
}
/**
 * @brief
 *
 * @param a
 * @param b
 * @return lnSPI*
 */
lnSPI *lnSPI::create(int instance, int pinCs)
{
    return new rpSPI(instance, pinCs);
}
//--- not implemented ---
bool rpSPI::transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn)
{
    xAssert(0);
    return false;
}
/**
 * @brief
 *
 * @return true
 * @return false
 */
bool rpSPI::finishAsyncDma()
{
    _txDma->endTransfer();
    _txDone.give();
    return true;
}
/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::asyncWrite8(int nbBytes, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    this->_callback = cb;
    this->_callbackCookie = cookie;
    // source targer
    _spi->DMACR = LN_RP_SPI_DMACR_TX;
    _spi->IMSC |= LN_RP_SPI_INT_TX;
    _txDone.tryTake();
    _txDma->attachCallback(dmaCb, this);
    _txDma->doMemoryToPeripheralTransferNoLock(nbBytes, (const uint32_t *)data, (const uint32_t *)&(_spi->DR), repeat);
    _txDma->beginTransfer();
    return true;
}
/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::nextWrite8(int nbBytes, const uint8_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    this->_callback = cb;
    this->_callbackCookie = cookie;
    _txDma->attachCallback(dmaCb, this);
    return _txDma->continueMemoryToPeripheralTransferNoLock(nbBytes, (const uint32_t *)data);
}
//--- not implemented ---

//--- not implemented ---
//--- not implemented ---

/**
 * @brief
 *
 * @param z
 * @return true
 * @return false
 */
bool rpSPI::write8(const uint8_t data)
{
    return write16(data);
}
/**
 * @brief
 *
 * @param data
 * @return true
 * @return false
 */
bool rpSPI::write16(const uint16_t data)
{
    int countdown = 1 * 1000 * 1000; // 100M/90k =>
    while (!(_spi->SR & LN_RP_SPI_SR_TFE))
    {
        __asm__("nop");
        if (--countdown == 0)
        {
            return false;
        }
    }
    _spi->DR = data;
    return true;
}

/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @return true
 * @return false
 */
bool rpSPI::blockWrite8Repeat(int nbBytes, const uint8_t data)
{
    _callback = nullptr;
    for (int i = 0; i < nbBytes; i++)
        write8(data);
    return true;
}

// --- not implemented ---
bool rpSPI::read1wire(int nbRead, uint8_t *rd)
{
    return false;
} // read, reuse MOSI

// -- not implemented --
bool rpSPI::waitForAsync()
{
    bool r = _txDone.take(60 * 1000);
    waitForCompletion();
    //--
    return r;
}
//-------------------------++
#ifdef NO_DMA
/**
 * @brief
 *
 * @param nbWord
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::asyncWrite16(int nbWord, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    if (repeat)
        for (int i = 0; i < nbWord; i++)
            write16(data[0]);
    else
        for (int i = 0; i < nbWord; i++)
            write16(data[i]);
    cb(cookie);
    return true;
}
/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::nextWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    if (repeat)
        for (int i = 0; i < nbBytes; i++)
            write16(data[0]);
    else
        for (int i = 0; i < nbBytes; i++)
            write16(data[i]);
    cb(cookie);
    return true;
}
/**
 * @brief
 *
 * @param nbHalfWord
 * @param data
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::blockWrite16Repeat(int nbHalfWord, const uint16_t data)
{
    for (int i = 0; i < nbHalfWord; i++)
        write16(data);
    return true;
}
//-------------------------++
#else

/**
 * @brief
 *
 * @param nbWord
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::asyncWrite16(int nbWord, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    this->_callback = cb;
    this->_callbackCookie = cookie;
    // source target
    _spi->DMACR = LN_RP_SPI_DMACR_TX;
    _spi->IMSC |= LN_RP_SPI_INT_TX;
    _txDone.tryTake();
    _txDma->setTransferSize(16);
    _txDma->attachCallback(dmaCb, this);
    _txDma->doMemoryToPeripheralTransferNoLock(nbWord, (const uint32_t *)data, (const uint32_t *)&(_spi->DR), repeat);
    _txDma->beginTransfer();
    return true;
}
/**
 * @brief
 *
 * @param nbBytes
 * @param data
 * @param cb
 * @param cookie
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::nextWrite16(int nbBytes, const uint16_t *data, lnSpiCallback *cb, void *cookie, bool repeat)
{
    this->_callback = cb;
    this->_callbackCookie = cookie;
    return _txDma->continueMemoryToPeripheralTransferNoLock(nbBytes, (const uint32_t *)data);
}
/**
 * @brief
 *
 * @param nbHalfWord
 * @param data
 * @param repeat
 * @return true
 * @return false
 */
bool rpSPI::blockWrite16Repeat(int nbHalfWord, const uint16_t data)
{
    xAssert(_wordSize == 16);
    _callback = nullptr;
    //_current = data;
    //_limit = _current + nbHalfWord;
    _spi->DMACR = LN_RP_SPI_DMACR_TX;
    _spi->IMSC |= LN_RP_SPI_INT_TX;
    _state = TxStateBody;
    _txDone.tryTake();
    _txDma->setTransferSize(8);
    _txDma->attachCallback(dmaCb, this);
    _txDma->doMemoryToPeripheralTransferNoLock(nbHalfWord, (const uint32_t *)&data, (const uint32_t *)&(_spi->DR),
                                               true);
    _txDma->beginTransfer();
    _txDone.take();
    _txDma->endTransfer();
    return true;
}
//-------------------------++
#endif
//--
