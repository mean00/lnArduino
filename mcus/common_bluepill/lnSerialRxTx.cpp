/**
 * @brief
 *
 */
class lnSerialBpRxTx : public lnSerialBpTxOnlyInterrupt, public lnSerialRxTx
{
  public:
    lnSerialBpRxTx(int instance, int bufferSize);
    virtual ~lnSerialBpRxTx();
    virtual bool transmit(int size, const uint8_t *buffer)
    {
        return lnSerialBpTxOnlyInterrupt::transmit(size, buffer);
    }
    virtual bool enableRx(bool enabled);
    virtual void purgeRx();
    virtual int read(int max, uint8_t *to);
    // no copy interface
    virtual int getReadPointer(uint8_t **to);
    virtual void consume(int n);
    virtual void _interrupt(void);
    void rxInterruptHandler(void);
    void enableRxInterrupt()
    {
        enableInterrupt(_txState == txTransmittingInterrupt || _txState == txTransmittingLast, true);
    }
    //
    int modulo(int in)
    {
        if (in >= _rxBufferSize)
            return in - _rxBufferSize;
        return in;
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
    int _rxBufferSize;
    int _rxHead, _rxTail;
    uint8_t *_rxBuffer;
    bool _rxEnabled;
    int _rxError;
    //
};

/**
 * @brief
 *
 */
void lnSerialBpRxTx::_interrupt(void)
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
#define ALL_INTERRUPTS2 LN_USART_STAT_RBNE + (LN_USART_STAT_TC + LN_USART_STAT_TBE + LN_USART_STAT_OVERR)
    volatile int stat = d->STAT & d->CTL0 & ALL_INTERRUPTS2; // filter enabled interrupt only
    if (stat & (LN_USART_STAT_TC + LN_USART_STAT_TBE))
    {
        if (_txState == txTransmittingInterrupt || _txState == txTransmittingLast)
            txInterruptHandler();
    }
    if (stat & (LN_USART_STAT_RBNE) && _rxEnabled)
    {
        rxInterruptHandler();
    }
    else if (stat & (LN_USART_STAT_NERR + LN_USART_STAT_PERR + LN_USART_STAT_FERR + LN_USART_STAT_ORERR)) // Error
    {
        lnScratchRegister = d->DATA;
    }
}
/**
 * @brief
 *
 */
void lnSerialBpRxTx::rxInterruptHandler(void)
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    volatile int stat = d->STAT;
    bool empty = (_rxTail == _rxHead);
    int pushed = 0;
    while (stat & (LN_USART_STAT_RBNE))
    {
        uint8_t c = d->DATA;
        if (stat & (LN_USART_STAT_FERR + LN_USART_STAT_NERR))
        {
            _rxError++;
        }
        else
        {
            // do we have space in the ring buffer ?
            int next = modulo(_rxTail + 1);
            if (next != _rxHead) // is it full ?
            {
                _rxBuffer[next] = c;
                _rxTail = next;
                pushed++;
            }
        }
        stat = d->STAT;
    }
    if (empty && pushed)
    {
        // wakeup receiver...
        if (_cb)
            _cb(_cbCookie, dataAvailable);
    }
}

/**
 * @brief Construct a new ln Serial Bp Rx Tx::ln Serial Bp Rx Tx object
 *
 * @param instance
 */
lnSerialBpRxTx::lnSerialBpRxTx(int instance, int bufferSize)
    : lnSerialBpTxOnlyInterrupt(instance), lnSerialRxTx(instance)
{
    _rxEnabled = false;
    _rxBufferSize = bufferSize;
    _rxBuffer = new uint8_t[_rxBufferSize];
}
/**
 * @brief Destroy the ln Serial Bp Rx Tx::ln Serial Bp Rx Tx object
 *
 */
lnSerialBpRxTx::~lnSerialBpRxTx()
{
    delete[] _rxBuffer;
    _rxBuffer = NULL;
}
/**
 * @brief
 *
 * @param to
 * @return int
 */
int lnSerialBpRxTx::getReadPointer(uint8_t **to)
{
    disableInterrupt();
    if (_rxHead == _rxTail) // source empty or target full
    {
        enableRxInterrupt();
        return 0;
    }
    int z = 0;
    if (_rxHead > _rxTail)
    {
        z = _rxBufferSize - _rxHead;
    }
    else
    {
        z = _rxTail - _rxHead;
    }
    *to = _rxBuffer + _rxHead;
    enableRxInterrupt();
    return z;
}
/**
 * @brief
 *
 * @param max
 * @param to
 * @return int
 */
int lnSerialBpRxTx::read(int max, uint8_t *to)
{
    int total = 0;
    while (1)
    {
        disableInterrupt();
        if ((_rxHead == _rxTail) || !max) // source empty or target full
        {
            enableRxInterrupt();
            return total;
        }
        int z = 0;
        if (_rxHead > _rxTail)
        {
            z = _rxBufferSize - _rxHead;
        }
        else
        {
            z = _rxTail - _rxHead;
        }
        if (z > max)
            z = max;
        memcpy(to, _rxBuffer + _rxHead, z);
        _rxHead = modulo(_rxHead + z);
        enableRxInterrupt();
        total += z;
        to += z;
        max -= z;
    }
    xAssert(0);
    return 0;
}
/**
 * @brief
 *
 */
void lnSerialBpRxTx::purgeRx()
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    disableInterrupt();
    _rxHead = _rxTail = 0;
    volatile uint32_t stat = d->STAT;
    while (stat & (LN_USART_STAT_RBNE))
    {
        lnScratchRegister = d->DATA;
        stat = d->STAT;
    }
    enableRxInterrupt();
}
/**
 * @brief
 *
 * @param enabled
 * @return true
 * @return false
 */
bool lnSerialBpRxTx::enableRx(bool enabled)
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    d->CTL0 &= ~LN_USART_CTL0_UEN;
    if (enabled)
    {
        disableInterrupt();
        _rxEnabled = true;
        d->CTL0 |= LN_USART_CTL0_RBNEIE;
        d->CTL0 |= LN_USART_CTL0_REN;
        enableInterrupt(false, true);
    }
    else
    {
        disableInterrupt();
        _rxEnabled = false;
        d->CTL0 &= ~LN_USART_CTL0_REN;
        d->CTL0 &= ~LN_USART_CTL0_RBNEIE;
        enableRxInterrupt();
    }
    d->CTL0 |= LN_USART_CTL0_UEN;
    return true;
}

/**
 * @brief
 *
 * @param n
 */
void lnSerialBpRxTx::consume(int n)
{
    disableInterrupt();
    _rxHead = modulo(_rxHead + n);
    enableRxInterrupt();
}

// EOF