/**
 * @file    rp_serial.cpp
 * @author  mean00
 * @brief
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "lnArduino.h"
#include "lnIRQ_arm.h"
#include "lnRingBuffer.h"
#include "lnSerial.h"

#include "hardware/irq.h"
#include "hardware/regs/intctrl.h"
#include "hardware/uart.h"
#include "ln_rp_dma.h"
#include "ln_rp_dma_channel_map.h"

#include "timers.h"

class lnRpSerialTxOnly;
static void uart0_irq_handler(void);
static void uart1_irq_handler(void);

static lnRpSerialTxOnly *rpSerialInstances[2] = {NULL, NULL};

#if 0
#define xParanoid xAssert
#else
#define xParanoid(...)                                                                                                 \
    {                                                                                                                  \
    }
#endif

/**
 * @brief     We dont use DMA for Rx
 *
 */
struct lnUart_t
{
    const uart_inst_t *hw;
    const lnRpDMA::LN_RP_DMA_DREQ dreq_tx;
    const lnRpDMA::LN_RP_DMA_DREQ dreq_rx;
    const int irq;
    irq_handler_t irq_handler;
};
/**
 * @brief
 *
 * @param u
 * @param size
 * @param buffer
 * @return true
 * @return false
 */
bool rp_uart_rawWrite(uart_inst_t *u, int size, const uint8_t *buffer)
{
    io_rw_32 *dr = &(uart_get_hw(u)->dr);
    for (int i = 0; i < size; i++)
    {
        while (!(uart_is_writable(u)))
        {
            __asm__("nop");
        }
        *dr = *buffer++;
    }
    return true;
}
/*
 */
static const lnUart_t uarts[2] = {{((const uart_inst_t *)uart0_hw), lnRpDMA::LN_DMA_DREQ_UART0_TX,
                                   lnRpDMA::LN_DMA_DREQ_UART0_RX, UART0_IRQ, uart0_irq_handler},
                                  {((const uart_inst_t *)uart1_hw), lnRpDMA::LN_DMA_DREQ_UART1_TX,
                                   lnRpDMA::LN_DMA_DREQ_UART1_RX, UART1_IRQ, uart1_irq_handler}};

/**
 * @brief
 *
 */

class lnRpSerialTxOnly : public lnSerialTxOnly
{
  public:
    // public API
    lnRpSerialTxOnly(int instance);
    virtual bool init();
    bool setSpeed(int speed);
    bool transmit(int size, const uint8_t *buffer);

    void disableInterrupt();
    void enableInterrupt(bool txInterruptEnabled);
    // no copy interface
    virtual bool rawWrite(int size, const uint8_t *buffer)
    {
        uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
        return rp_uart_rawWrite(u, size, buffer);
    }

    // custom
    virtual void irq_handler();
    bool transmitIrq(int size, const uint8_t *buffer);
    bool transmitDma(int size, const uint8_t *buffer);
    bool transmitPolling(int size, const uint8_t *buffer);
    void txDmaCb();
    int txLimit;
    int txCurrent;
    const uint8_t *_txData;
    lnRpDMA *_txDma;
    lnBinarySemaphore _txDone;
    // our
    void baseUartInit()
    {
        uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;

        uart_init(u, 115200);
        uart_set_hw_flow(u, false, false);
        uart_set_format(u, 8, 1, UART_PARITY_NONE);
        uart_set_fifo_enabled(u, false);
        irq_set_exclusive_handler(uarts[lnSerialTxOnly::_instance].irq, uarts[lnSerialTxOnly::_instance].irq_handler);
        uart_set_irq_enables(u, false, false); // disable Rx & Tx
    }
};
/**
 * @brief
 *
 */
void uart0_irq_handler()
{
    xAssert(rpSerialInstances[0]);
    rpSerialInstances[0]->irq_handler();
}
/**
 * @brief
 *
 */
void uart1_irq_handler()
{
    xAssert(rpSerialInstances[1]);
    rpSerialInstances[1]->irq_handler();
}

/**
    \fn
    \brief
*/
lnRpSerialTxOnly::lnRpSerialTxOnly(int instance) : lnSerialTxOnly(instance)
{
    xAssert(instance < 2);
    xAssert(!rpSerialInstances[instance]) _txDma = NULL;
    rpSerialInstances[instance] = this;
}

static void cbTxDma(void *cookie)
{
    lnRpSerialTxOnly *serial = (lnRpSerialTxOnly *)cookie;
    serial->txDmaCb();
}
/**
 * @brief
 *
 */
void lnRpSerialTxOnly::txDmaCb()
{
    _txDma->endTransfer();
    _txDone.give();
}
/**
    \fn
    \brief
*/
bool lnRpSerialTxOnly::init()
{
    baseUartInit();
    // ok initialize DMA, we are only Txing
    const lnUart_t *t = uarts + lnSerialTxOnly::_instance;
    _txDma = new lnRpDMA(lnRpDMA::DMA_MEMORY_TO_PERIPH, t->dreq_tx,
                         8); // can we do 8bytes access ?
    _txDma->attachCallback(cbTxDma, this);
    // cut the irq at nvic level, if we use DMA we only need the DMA interrutp
    irq_set_enabled(uarts[lnSerialTxOnly::_instance].irq, false);
    return true;
}
/**
    \fn
    \brief
*/
bool lnRpSerialTxOnly::setSpeed(int speed)
{
    uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
    uart_set_baudrate(u, speed);
    return true;
}
/**
    \fn
    \brief
*/
bool lnRpSerialTxOnly::transmit(int size, const uint8_t *buffer)
{
    return transmitIrq(size, buffer);
}

/**
 * @brief
 *
 * @param size
 * @param buffer
 * @return true
 * @return false
 */
bool lnRpSerialTxOnly::transmitPolling(int size, const uint8_t *buffer)
{

    uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
    io_rw_32 *dr = &(uart_get_hw(u)->dr);
    for (int i = 0; i < size; i++)
    {
        while (!(uart_is_writable(u)))
        {
            __asm__("nop");
        }
        *dr = *buffer++;
    }

    return true;
}

/**
 * @brief
 *
 * @param size
 * @param buffer
 * @return true
 * @return false
 */
bool lnRpSerialTxOnly::transmitIrq(int size, const uint8_t *buffer)
{

    // Pre-fill the uart
    txLimit = size;
    txCurrent = 0;
    _txData = buffer;
    uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
    io_rw_32 *dr = &(uart_get_hw(u)->dr);
    while ((uart_is_writable(u)) && txCurrent < txLimit)
        *dr = _txData[txCurrent++];
    // if more to do, let the irq handle it
    if (txCurrent != txLimit)
    {
        irq_set_enabled(uarts[lnSerialTxOnly::_instance].irq, true); // enable nvic IRQ
        uart_set_irq_enables(u, false, true);                        // enable Tx
        _txDone.take();
    }
    return true;
}
/**
    \fn
    \brief
*/
bool lnRpSerialTxOnly::transmitDma(int size, const uint8_t *buffer)
{
    if (!size)
        return true;
    xAssert(_txDma);
    uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
    _txDma->doMemoryToPeripheralTransferNoLock(size, (const uint32_t *)buffer, (const uint32_t *)&(uart_get_hw(u)->dr));
    _txDma->beginTransfer();
    _txDone.take();
    return true;
}

/**
 * @brief
 *
 */
void lnRpSerialTxOnly::irq_handler()
{
    uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
    io_rw_32 *dr = &(uart_get_hw(u)->dr);
    while ((uart_is_writable(u)) && txCurrent < txLimit)
        *dr = _txData[txCurrent++];
    if (txCurrent == txLimit)
    {
        uart_set_irq_enables(u, false, false); // disable Tx
        _txDone.give();
    }
}
/**
    \fn
    \brief
*/
void lnRpSerialTxOnly::disableInterrupt()
{
}
/**
    \fn
    \brief
*/
void lnRpSerialTxOnly::enableInterrupt(bool txInterruptEnabled)
{
}
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
/**
 * @brief
 *
 */
class lnRpSerialRxTx : public lnRpSerialTxOnly, public lnSerialRxTx
{
  public:
    // public API
    lnRpSerialRxTx(int instance, int rxBufferSize = 128);
    virtual bool init();
    bool setSpeed(int speed)
    {
        return lnRpSerialTxOnly::setSpeed(speed);
    }
    bool transmit(int size, const uint8_t *buffer)
    {
        return lnRpSerialTxOnly::transmit(size, buffer);
    }
    bool enableRx(bool enabled);
    void purgeRx();
    int read(int max, uint8_t *to);
    // no copy interface
    int getReadPointer(uint8_t **to);
    void consume(int n);

    void copyToBuffer(uint32_t startFrom, uint32_t count);
    void timer();
    lnRpDMA *_rxDma;
    void rxDmaCb();
#define UART_RX_DMA_BUFFER 64
    uint8_t _rxDmaBuffer[UART_RX_DMA_BUFFER];
    int _rxDmaLastIndex;
    TimerHandle_t _cleanupTimer;
    lnRingBuffer _ring;
};

/**
 * @brief
 *
 * @param cookie
 */
static void cbRxDma(void *cookie)
{
    lnRpSerialRxTx *serial = (lnRpSerialRxTx *)cookie;
    serial->rxDmaCb();
}
/**
 * @brief
 *
 * @param t
 */
static void timerCleanupCallback(TimerHandle_t t)
{
    lnRpSerialRxTx *serial = (lnRpSerialRxTx *)pvTimerGetTimerID(t);
    serial->timer();
}

/**
 * @brief Construct a new ln Rp Serial Rx Tx::ln Rp Serial Rx Tx object
 *
 * @param instance
 * @param rxBufferSize
 */
lnRpSerialRxTx::lnRpSerialRxTx(int instance, int rxBufferSize)
    : lnRpSerialTxOnly(instance), _ring(rxBufferSize), lnSerialRxTx(instance)
{
    _cleanupTimer = xTimerCreate(
        "uart",
        10, // the dma buffer is 64 bytes @ 115200, that means about 5ms to fill the buffer, we want something bigger
            // if the speed is lower, it does not matter much, we'll just wait cycles a bit
        true, this, timerCleanupCallback);
}
/**
 * @brief the dma will trigger only when it has receive the full amount of data it needs
 *  we have a 1ms timer that from time to time will empty the buffer
 *  It works fine as long as the serial speed is above 1m vs 64 bytes , i.e. 15 us , below 500 kB/s
 *  This is called from within the timer task context, so we can be interrupted by any interrupt
 */
void lnRpSerialRxTx::timer()
{
    // block DMA interrupts so the DMA does not come in
    _rxDma->disableInterrupt();
    uint32_t val = UART_RX_DMA_BUFFER - _rxDma->getCurrentCount(); // total # of received bytes
    //
    uint32_t toCopy = val - _rxDmaLastIndex;
    if (!toCopy)
    {
        _rxDma->enableInterrupt(); // nothing new...
        return;
    }

    // copy to buffer
    copyToBuffer(_rxDmaLastIndex, toCopy);
    _rxDmaLastIndex = val;
    _rxDma->enableInterrupt();
}
/**
 * @brief
 *
 */
void lnRpSerialRxTx::rxDmaCb()
{
    // the timer is called from a task,  no need to block it
    uint32_t val = UART_RX_DMA_BUFFER; // total # of received bytes
    uint32_t toCopy = val - _rxDmaLastIndex;
    // copy to copy
    uint32_t copyFrom = _rxDmaLastIndex;
    _rxDmaLastIndex = 0;
    uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
    _rxDma->continuePeripheralToMemoryTransferNoLock(UART_RX_DMA_BUFFER, (const uint32_t *)_rxDmaBuffer);
    _rxDma->beginTransfer(); // next!

    if (toCopy) // we assume we'll be faster than the DMA
    {
        copyToBuffer(copyFrom, toCopy);
    }
    BaseType_t wake = pdFALSE;
    // reset the timer, we just purged the buffer
    xTimerResetFromISR(_cleanupTimer, &wake);
    return;
}

/**
 * @brief
 *
 * @param startFrom
 * @param count
 */
void lnRpSerialRxTx::copyToBuffer(uint32_t startFrom, uint32_t count)
{
    bool was_empty = _ring.empty();
    xParanoid(startFrom + count <= _ring.size()) _ring.put(count, _rxDmaBuffer + startFrom);
    if (was_empty && count)
    {
        xAssert(_cb);
        _cb(_cbCookie, lnSerialCore::dataAvailable);
    }
}

/**
 * @brief
 *
 * @param enabled
 * @return true
 * @return false
 */
bool lnRpSerialRxTx::enableRx(bool enabled)
{
    if (enabled == false)
    {
        _rxDma->cancelTransfer();
        xTimerStop(_cleanupTimer, 1);
    }
    else
    {
        uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
        _rxDmaLastIndex = 0;
        _rxDma->doPeripheralToMemoryTransferNoLock(UART_RX_DMA_BUFFER, (const uint32_t *)&(uart_get_hw(u)->dr),
                                                   (const uint32_t *)_rxDmaBuffer);
        _rxDma->beginTransfer();
        xTimerStart(_cleanupTimer, 1); // start the cleanup timer
    }
    return true;
}
/**
 * @brief  Empty the Rx data in flight
 *
 */
void lnRpSerialRxTx::purgeRx()
{
    uart_inst_t *u = (uart_inst_t *)uarts[lnSerialTxOnly::_instance].hw;
    while (uart_is_readable(u))
        uart_getc(u);
    _ring.flush();
}
/**
 * @brief
 *
 * @param max
 * @param to
 * @return int
 */

// we should not be needing this, to be optimized later...
#define DISABLE_INTERRUPTS() taskENTER_CRITICAL()
#define ENABLE_INTERRUPTS() taskEXIT_CRITICAL()

int lnRpSerialRxTx::read(int max, uint8_t *to)
{
    DISABLE_INTERRUPTS();
    int r = _ring.get(max, to);
    ENABLE_INTERRUPTS();
    return r;
}
/**
 * @brief
 *
 * @param to
 * @return int
 */
int lnRpSerialRxTx::getReadPointer(uint8_t **to)
{
    DISABLE_INTERRUPTS();
    int r = _ring.getReadPointer(to);
    ENABLE_INTERRUPTS();
    return r;
}
/**
 * @brief
 *
 * @param n
 */
void lnRpSerialRxTx::consume(int n)
{
    DISABLE_INTERRUPTS();
    _ring.consume(n);
    ENABLE_INTERRUPTS();
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool lnRpSerialRxTx::init()
{
    const lnUart_t *t = uarts + lnSerialTxOnly::_instance;
    lnRpSerialTxOnly::init();
    _rxDma = new lnRpDMA(lnRpDMA::DMA_PERIPH_TO_MEMORY, t->dreq_rx, 8);
    _rxDma->attachCallback(cbRxDma, this);
    _rxDmaLastIndex = 0;
    return true;
}

/**
 * @brief Create a Ln Serial Tx Only object
 *
 * @param instance
 * @param dma
 * @param buffered
 * @return lnSerialTxOnly*
 */
lnSerialTxOnly *createLnSerialTxOnly(int instance, bool dma, bool buffered)
{
    return new lnRpSerialTxOnly(instance);
}
/**
 * @brief Create a Ln Serial Rx Tx object
 *
 * @param instance
 * @param rxBufferSize
 * @param dma
 * @return lnSerialRxTx*
 */
lnSerialRxTx *createLnSerialRxTx(int instance, int rxBufferSize, bool dma)
{
    return new lnRpSerialRxTx(instance, rxBufferSize);
}
// EOF