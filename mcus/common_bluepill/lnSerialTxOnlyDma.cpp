class lnSerialBpTxOnlyDma : public lnSerialBpCore, public lnSerialTxOnly
{
  public:
    lnSerialBpTxOnlyDma(int instance);
    virtual bool _programTx();
    bool init()
    {
        return lnSerialBpCore::init();
    }
    bool setSpeed(int speed)
    {
        return lnSerialBpCore::setSpeed(speed);
    }
    virtual bool transmit(int size, const uint8_t *buffer);
    virtual void _interrupt(void);
    virtual bool rawWrite(int size, const uint8_t *buffer);
};

#if 0
/**
 *
 * @param size
 * @param buffer
 * @return
 */
bool lnSerialBpCore::transmit(int size, const uint8_t *buffer)
{
    // return true;
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    _txMutex.lock();
    ENTER_CRITICAL();    
    _tail = buffer + size;
    _cur = buffer + 1;
    if (size == 1)
    {
        _txState = txTransmittingLast;
    }
    else
    {
        _txState = txTransmittingInterrupt;
    }
    d->STAT &= ~(LN_USART_STAT_TC);
    // send 1st byte
    d->DATA = (uint32_t)buffer[0];
    // enable TB interrupt
    _programTx();
    EXIT_CRITICAL();
    _txDone.take();
    _txMutex.unlock();
    return true;
}

/**
 *
 */
void lnSerialBpCore::txDmaCb()
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    // disable TC & TBE
    d->CTL0 &= ~(LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
    // clear TC
    // nope d->STAT&=~(LN_USART_STAT_TC);
    // clear DMA
    d->CTL2 &= ~(LN_USART_CTL2_DMA_TX);
    _txState = txTransmittingIdle;
    _txDone.give();
}
/**
 *
 * @param c
 */
void lnSerialBpCore::_dmaCallback(void *c, lnDMA::DmaInterruptType it)
{
    lnSerialBp *i = (lnSerialBp *)c;
    i->txDmaCb();
}

/**
 *
 * @param size
 * @param buffer
 * @return
 */
bool lnSerialBpCore::dmaTransmit(int size, const uint8_t *buffer)
{
    // return true;
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    _txMutex.lock();        // lock uart
    _txDma.beginTransfer(); // lock dma
    ENTER_CRITICAL();
    _txState = txTransmittingDMA;
    _programTx();
    d->STAT &= ~LN_USART_STAT_TC;
    _txDma.attachCallback(_dmaCallback, this);
    EXIT_CRITICAL();

    _txDma.doMemoryToPeripheralTransferNoLock(size, (uint16_t *)buffer, (uint16_t *)&(d->DATA), false);

    _txDone.take();
    _txDma.endTransfer();

    // Wait busy bit to clear out
    while (!(d->STAT & LN_USART_STAT_TC))
    {
        __asm__("nop" ::);
    }
    d->STAT &= ~(LN_USART_STAT_TC);

    _txMutex.unlock();
    return true;
}

/**

*/
void lnSerialBpCore::txInterruptHandler(void)
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    volatile int stat = d->STAT;
    lnScratchRegister = d->CTL0;
    switch (_txState)
    {
    case txTransmittingInterrupt:
        xAssert(stat & LN_USART_STAT_TBE);
        d->DATA = (uint32_t)*_cur;
        _cur++;
        if (_cur > _tail)
            xAssert(0);
        if (_cur == _tail)
        {
            d->CTL0 |= LN_USART_CTL0_TCIE;
            d->CTL0 &= ~(LN_USART_CTL0_TBIE); // only let the Transmission complete bit
            _txState = txTransmittingLast;
        }
        return;
        break;
    case txTransmittingLast:
        if (!(stat & LN_USART_STAT_TC))
            xAssert(0); // we got here due to RX interrupt...
        d->CTL0 &= ~(LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
        d->STAT &= ~(LN_USART_STAT_TC);
        _txState = txTransmittingIdle;
        _txDone.give();
        return;
        break;
    default:
        xAssert(0);
        break;
    }
}
#endif
#if 0
/**
 *
 * @return
 */
bool lnSerialBpTxOnly::_programTx()
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    d->CTL0 &= ~LN_USART_CTL0_UEN;
    switch (_txState)
    {
    case txTransmittingLast:
        disableInterrupt();
        // disable TC & TE tx interrupt
        d->CTL0 &= ~(LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
        // enable Tx
        d->CTL2 &= ~LN_USART_CTL2_DMA_TX; // disable TX DMA
        d->CTL0 |= LN_USART_CTL0_TEN;     // enable TX
        d->CTL0 |= LN_USART_CTL0_TCIE;    // enable TBIE
        enableInterrupt(true);            // enable tx part of interrupt
        break;
    case txTransmittingInterrupt:
        disableInterrupt();
        // disable TC & TE tx interrupt
        d->CTL0 &= ~(LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
        // enable Tx
        d->CTL2 &= ~LN_USART_CTL2_DMA_TX; // disable TX DMA
        d->CTL0 |= LN_USART_CTL0_TEN;     // enable TX
        d->CTL0 |= LN_USART_CTL0_TBIE;    // enable TBIE
        enableInterrupt(true);            // enable tx part of interrupt
        break;
    case txTransmittingIdle:
        disableInterrupt();
        d->CTL0 &= ~LN_USART_CTL0_TEN;    // disable TX
        d->CTL2 &= ~LN_USART_CTL2_DMA_TX; // disable TX DMA
        d->CTL0 &= ~(LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
        enableInterrupt(false); // disable tx part of interrupt
        break;   
    default:
        xAssert(0);
        break;
    }
    d->CTL0 |= LN_USART_CTL0_UEN;
    return true;
}
#endif
#if 0

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool lnSerialBpTxOnlyDma::_programTx()
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    d->CTL0 &= ~LN_USART_CTL0_UEN;
    switch (_txState)
    {
    case txTransmittingLast:
        disableInterrupt();
        // disable TC & TE tx interrupt
        d->CTL0 &= ~(LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
        // enable Tx
        d->CTL2 &= ~LN_USART_CTL2_DMA_TX; // disable TX DMA
        d->CTL0 |= LN_USART_CTL0_TEN;     // enable TX
        d->CTL0 |= LN_USART_CTL0_TCIE;    // enable TBIE
        enableInterrupt(true);            // enable tx part of interrupt
        break;  
    case txTransmittingDMA:
        disableInterrupt();
        d->CTL0 |= (LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
        d->CTL0 |= LN_USART_CTL0_TEN;    // enable TX
        d->CTL2 |= LN_USART_CTL2_DMA_TX; // enable TX DMA
        enableInterrupt(false);
        break;
    default:
        xAssert(0);
        break;
    }
    d->CTL0 |= LN_USART_CTL0_UEN;
    return true;
}
//------------------------------------------
const unsigned short int *_ctype_b;
/**
 * @brief Create a Ln Serial object
 * 
 * @param instance 
 * @param rxBufferSize 
 * @return lnSerialCore* 
 */
lnSerialCore *createLnSerial(int instance, lnSerialCore::lnSerialMode mode, int rxBufferSize)
{
    return new lnSerialBp(instance,mode, rxBufferSize);
}
#endif