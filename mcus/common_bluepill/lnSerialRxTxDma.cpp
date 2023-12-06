/**
 * @brief
 *
 */
#include "lnRingBuffer.h"

/**
 * @brief This is to avoid non virtual thunk gdb does not like that
 *
 */
class lnSerialBpRxTxDma;
class timerLink : public lnPeriodicTimer
{
  public:
    timerLink(lnSerialBpRxTxDma *parent) : lnPeriodicTimer()
    {
        _parent = parent;
    }
    lnSerialBpRxTxDma *_parent;
    void timerCallback();
};

class lnSerialBpRxTxDma : public lnSerialBpTxOnlyDma, public lnSerialRxTx
{
  public:
    lnSerialBpRxTxDma(int instance, int bufferSize);
    virtual ~lnSerialBpRxTxDma();
    bool init()
    {
        return lnSerialBpCore::init();
    }
    bool setSpeed(int speed)
    {
        return lnSerialBpCore::setSpeed(speed);
    }
    virtual bool rawWrite(int count, const uint8_t *buffer)
    {
        LN_USART_Registers *d = (LN_USART_Registers *)_adr;
        return ln_serial_rawWrite(d, count, buffer);
    }
    //----
    bool transmit(int size, const uint8_t *buffer);
    static void _rxDmaCb(void *me, lnDMA::DmaInterruptType type);
    virtual bool enableRx(bool enabled);
    virtual void purgeRx();
    virtual int read(int max, uint8_t *to);
    // no copy interface
    virtual int getReadPointer(uint8_t **to);
    virtual void consume(int n);
    //
    static void _dmaCallback2(void *c, lnDMA::DmaInterruptType it);
    void txDmaCb2(lnDMA::DmaInterruptType it);
    void rxInterruptHandler(void);
    void enableRxDmaInterrupt();
    void disableRxDmaInterrupt();
    void startRxDma();
    void timerCallback();
    virtual void _interrupt(void);

    //---
    static void _RxdmaCallback(void *c, lnDMA::DmaInterruptType it);
    void rxDma(lnDMA::DmaInterruptType type);

    void igniteTx();

    void checkForNewData();
    timerLink _timer;
    int _rxBufferSize;
    int _rxMask = 0;
    int _rxHead, _rxTail;
    bool _rxEnabled;
    int _inFlight;
    lnDMA _rxDma;
    uint8_t *_rxBuffer;
    lnRingBuffer _txRingBuffer;
    lnBinarySemaphore _txRingSem;
    bool _txing;
};
/**
 * @brief
 *
 * @param c
 * @param it
 */

void lnSerialBpRxTxDma::_dmaCallback2(void *c, lnDMA::DmaInterruptType it)
{
    lnSerialBpRxTxDma *i = (lnSerialBpRxTxDma *)c;
    i->txDmaCb2(it);
}
/**
 * @brief
 *
 * @param it
 */
void lnSerialBpRxTxDma::txDmaCb2(lnDMA::DmaInterruptType it)
{
    uint8_t *to;
    _txRingBuffer.consume(_inFlight);
    _inFlight = 0;
    _txRingSem.give(); // space freed, wake up thread

    int nb = _txRingBuffer.getReadPointer(&to);
    if (!nb) // done !
    {
        _txing = false;
        return;
    }
    // send remainer
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    _inFlight = nb;
    _txDma.doMemoryToPeripheralTransferNoLock(nb, (uint16_t *)to, (uint16_t *)&(d->DATA), false);
}
/**
 * @brief
 *
 * @param size
 * @param buffer
 * @return true
 * @return false
 */
bool lnSerialBpRxTxDma::transmit(int size, const uint8_t *buffer)
{
    while (size)
    {
        ENTER_CRITICAL();
        int nb = _txRingBuffer.free();
        if (!nb)
        {
            _txRingSem.tryTake(); // wait for a fresh sem post, not an old one
            EXIT_CRITICAL();
            _txRingSem.take();
            continue;
        }
        if (nb > size)
            nb = size;
        _txRingBuffer.put(nb, buffer);
        buffer += nb, size -= nb;
        EXIT_CRITICAL();
        if (!_txing)
            igniteTx();
    }
    return true;
}
/**
 * @brief
 *
 */
void lnSerialBpRxTxDma::igniteTx() // if we get here, no active transer if going on
{
    uint8_t *to = NULL;
    int nb = _txRingBuffer.getReadPointer(&to);
    xAssert(nb);
    _inFlight = nb;
    _txing = true;
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    _txDma.endTransfer();   // clear the previous one (if any)
    _txDma.beginTransfer(); // lock dma
    ENTER_CRITICAL();
    _txState = txTransmittingDMA;
    _programTx();
    d->STAT &= ~LN_USART_STAT_TC;
    _txDma.attachCallback(_dmaCallback2, this);
    EXIT_CRITICAL();

    _txDma.doMemoryToPeripheralTransferNoLock(nb, (uint16_t *)to, (uint16_t *)&(d->DATA), false);
}
/*
 */

/**
 * @brief
 *
 */
void lnSerialBpRxTxDma::_interrupt(void)
{
    xAssert(0);
}

lnSerialBpRxTxDma::lnSerialBpRxTxDma(int instance, int bufferSize)
    : lnSerialBpTxOnlyDma(instance), lnSerialRxTx(instance),
      _rxDma(lnDMA::DMA_PERIPH_TO_MEMORY, M(dmaEngine), M(dmaRxChannel), 32, 8), _timer(this),
      _txRingBuffer(bufferSize >> 1)
{
    _rxBuffer = new uint8_t[bufferSize];
    _rxEnabled = false;
    _rxBufferSize = bufferSize;
    _rxDma.attachCallback(_rxDmaCb, this);
    _rxHead = _rxTail = 0;
    _rxMask = _rxBufferSize - 1;
    _timer.init("uart", 20);
    _txing = false;
    xAssert((_rxBufferSize & (~_rxMask)) == _rxBufferSize); // must be a power of 2
}
/**
 * @brief Destroy the ln Serial Bp Rx Tx::ln Serial Bp Rx Tx object
 *
 */
lnSerialBpRxTxDma::~lnSerialBpRxTxDma()
{
    delete[] _rxBuffer;
}
/**
 * @brief
 *
 * @param max
 * @param to
 * @return int
 */
int lnSerialBpRxTxDma::read(int max, uint8_t *to)
{
    uint8_t *ptr;
    int nb = getReadPointer(&ptr);
    memcpy(to, ptr, nb);
    consume(nb);
    return nb;
}
/**
 * @brief
 *
 */
void lnSerialBpRxTxDma::purgeRx()
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    disableRxDmaInterrupt();
    _rxHead = _rxTail = 0;
    volatile uint32_t stat = d->STAT;
    while (stat & (LN_USART_STAT_RBNE))
    {
        lnScratchRegister = d->DATA;
        stat = d->STAT;
    }
}
/**
 * @brief
 *
 * @param enabled
 * @return true
 * @return false
 */
bool lnSerialBpRxTxDma::enableRx(bool enabled)
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    d->CTL0 &= ~LN_USART_CTL0_UEN;
    if (enabled)
    {
        disableRxDmaInterrupt();
        _rxDma.pause();
        _rxEnabled = true;
        d->CTL0 |= LN_USART_CTL0_RBNEIE;
        d->CTL0 |= LN_USART_CTL0_REN;
        d->CTL2 |= LN_USART_CTL2_DMA_RX;

        _timer.start();
        startRxDma();
    }
    else
    {
        disableRxDmaInterrupt();
        _rxDma.pause();
        _timer.stop();
        _rxEnabled = false;
        d->CTL0 &= ~LN_USART_CTL0_REN;
        d->CTL0 &= ~LN_USART_CTL0_RBNEIE;
        d->CTL2 &= ~LN_USART_CTL2_DMA_RX;
    }
    d->CTL0 |= LN_USART_CTL0_UEN;
    return true;
}
/**
 * @brief
 *
 */
void lnSerialBpRxTxDma::timerCallback()
{
    ENTER_CRITICAL(); // we are in a task, need to block interrupt and other tasks
    checkForNewData();
    EXIT_CRITICAL();
}
/**
 * @brief
 *
 * @return true
 * @return false
 */
void lnSerialBpRxTxDma::enableRxDmaInterrupt()
{
    _rxDma.enableInterrupt();
}
/**
 * @brief
 *
 * @return true
 * @return false
 */
void lnSerialBpRxTxDma::disableRxDmaInterrupt()
{
    _rxDma.disableInterrupt();
}
/**
 * @brief
 *
 */
void lnSerialBpRxTxDma::startRxDma()
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    _rxDma.attachCallback(_rxDmaCb, this);
    _rxDma.beginTransfer(); // lock dma
    _rxDma.doPeripheralToMemoryTransferNoLock(_rxBufferSize, (const uint16_t *)_rxBuffer, (const uint16_t *)&(d->DATA),
                                              true, true);
}

/**
 * @brief Construct a new ln Serial Bp Rx Tx::ln Serial Bp Rx Tx object
 *
 * @param instance
 */
void lnSerialBpRxTxDma::_rxDmaCb(void *me, lnDMA::DmaInterruptType type)
{
    lnSerialBpRxTxDma *serial = (lnSerialBpRxTxDma *)me;
    serial->rxDma(type);
}

/**
 * @brief
 *
 * @param to
 * @return int
 */
int lnSerialBpRxTxDma::getReadPointer(uint8_t **to)
{
    ENTER_CRITICAL();
    int h = _rxHead & _rxMask;
    int t = _rxTail & _rxMask;
    EXIT_CRITICAL();

    int nb;
    if (h >= t)
    {
        nb = h - t;
    }
    else
    {
        nb = _rxBufferSize - (t);
    }
    *to = _rxBuffer + t;
    return nb;
}
/**
 * @brief
 *
 * @param n
 */
void lnSerialBpRxTxDma::consume(int n)
{
    ENTER_CRITICAL();
    _rxTail += n;
    xAssert(_rxTail <= _rxBufferSize);
    _rxTail &= _rxMask;
    EXIT_CRITICAL();
}

//---
void lnSerialBpRxTxDma::checkForNewData()
{
    if (!_rxEnabled)
        return;
    int count = _rxDma.getCurrentCount(); // between N and 1, 0 means transfer done
    count = _rxBufferSize - count;        // this is the # of bytes transfered already
    uint32_t maskedHead = _rxHead & _rxMask;
    if (count != maskedHead)
    {
        _rxHead = count;
        xAssert(_cb);
        _cb(_cbCookie, lnSerialCore::dataAvailable);
    }
}
/**
 * @brief
 *
 * @param type
 */
void lnSerialBpRxTxDma::rxDma(lnDMA::DmaInterruptType type)
{
    int current = _rxHead;
    bool newData = false;
    switch (type)
    {
    case lnDMA::DMA_INTERRUPT_HALF:
        if (current != _rxBufferSize >> 1)
        {
            newData = true;
            _rxHead = _rxBufferSize >> 1;
        }
        break;
    case lnDMA::DMA_INTERRUPT_FULL:
        if (current != 0)
        {
            newData = true;
            _rxHead = 0;
        }
        break;
    default:
        xAssert(0);
    }
    if (newData)
    {
        _timer.restart();
        xAssert(_cb);
        _cb(_cbCookie, lnSerialCore::dataAvailable);
    }
}
/**
 * @brief
 *
 */
void timerLink::timerCallback()
{
    _parent->timerCallback();
}

// EOF