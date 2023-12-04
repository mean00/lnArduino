/**
 * @brief
 *
 */

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
    virtual bool transmit(int size, const uint8_t *buffer)
    {
        return lnSerialBpTxOnlyDma::transmit(size, buffer);
    }
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
    static void _rxDmaCb(void *me, lnDMA::DmaInterruptType type);
    virtual bool enableRx(bool enabled);
    virtual void purgeRx();
    virtual int read(int max, uint8_t *to);
    // no copy interface
    virtual int getReadPointer(uint8_t **to);
    virtual void consume(int n);
    //

    void rxInterruptHandler(void);
    void enableRxDmaInterrupt();
    void disableRxDmaInterrupt();
    void startRxDma();
    void timerCallback();
    virtual void _interrupt(void);

    //---
    static void _RxdmaCallback(void *c, lnDMA::DmaInterruptType it);
    void rxDma(lnDMA::DmaInterruptType type);

    void checkForNewData();
    timerLink _timer;
    int _rxBufferSize;
    int _rxMask = 0;
    int _rxHead, _rxTail;
    bool _rxEnabled;
    lnDMA _rxDma;
    uint8_t *_rxBuffer;
};

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
      _rxDma(lnDMA::DMA_PERIPH_TO_MEMORY, M(dmaEngine), M(dmaRxChannel), 32, 8), _timer(this)
{
    _rxBuffer = new uint8_t[bufferSize];
    _rxEnabled = false;
    _rxBufferSize = bufferSize;
    _rxDma.attachCallback(_rxDmaCb, this);
    _rxHead = _rxTail = 0;
    _rxMask = _rxBufferSize - 1;
    _timer.init("uart", 20);
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
    int h = _rxHead;
    int t = _rxTail;
    EXIT_CRITICAL();

    int nb;
    if (h >= t)
    {
        nb = h - t;
    }
    else
    {
        nb = _rxBufferSize - t;
    }
    *to = _rxBuffer + (t & _rxMask);
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
    if (_rxTail >= _rxBufferSize)
    {
        _rxTail -= _rxBufferSize;
        _rxHead -= _rxBufferSize;
    }
    EXIT_CRITICAL();
}

//---
void lnSerialBpRxTxDma::checkForNewData()
{
    if (!_rxEnabled)
        return;
    int count = _rxDma.getCurrentCount(); // between N and 1, 0 means transfer done
    count = _rxBufferSize - count;
    if (count != _rxHead)
    {
        if (_rxHead >= _rxTail)
        {
            _rxHead = count + (_rxHead & ~_rxMask);
        }
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
    int current = _rxHead & _rxMask;
    bool newData = false;
    switch (type)
    {
    case lnDMA::DMA_INTERRUPT_HALF:
        if (current != ((_rxBufferSize / 2) - 1))
        {
            newData = true;
            _rxHead = (_rxHead & ~_rxMask) + (_rxBufferSize >> 1);
        }
        break;
    case lnDMA::DMA_INTERRUPT_FULL:
        if (current != (_rxBufferSize - 1))
        {
            newData = true;
            _rxHead = (_rxHead & ~_rxMask) + _rxBufferSize;
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