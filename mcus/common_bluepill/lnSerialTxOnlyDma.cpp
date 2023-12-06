#define M(x) usartMapping[instance].x

/**
 */
#include "lnDma.h"
/**
 * @brief
 *
 */
class lnSerialBpTxOnlyDma : public lnSerialBpCore, public lnSerialTxOnly
{
  public:
    lnSerialBpTxOnlyDma(int instance);

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
    virtual bool transmit(int size, const uint8_t *buffer);
    virtual void _interrupt(void)
    {
        xAssert(0);
    }
    bool transmitMe(int size, const uint8_t *buffer, lnDMA::doneCallback *c);
    virtual bool _programTx();
    void txDmaCb();
    static void _dmaCallback(void *c, lnDMA::DmaInterruptType it);
    lnDMA _txDma;
};
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
    case txTransmittingDMA:
        disableInterrupt();
        d->CTL0 |= (LN_USART_CTL0_TBIE + LN_USART_CTL0_TCIE);
        d->CTL0 |= LN_USART_CTL0_TEN;    // enable TX
        d->CTL2 |= LN_USART_CTL2_DMA_TX; // enable TX DMA
        enableInterrupt(false, false);
        break;
    default:
        xAssert(0);
        break;
    }
    d->CTL0 |= LN_USART_CTL0_UEN;
    return true;
}

/**
 * @brief Construct a new ln Serial Bp Tx Only Dma object
 *
 * @param instance
 */
lnSerialBpTxOnlyDma::lnSerialBpTxOnlyDma(int instance)
    : lnSerialTxOnly(instance), lnSerialBpCore(instance),
      _txDma(lnDMA::DMA_MEMORY_TO_PERIPH, M(dmaEngine), M(dmaTxChannel), 8, 32)
{
}

/**
 * @brief
 *
 */
void lnSerialBpTxOnlyDma::txDmaCb()
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
void lnSerialBpTxOnlyDma::_dmaCallback(void *c, lnDMA::DmaInterruptType it)
{
    lnSerialBpTxOnlyDma *i = (lnSerialBpTxOnlyDma *)c;
    i->txDmaCb();
}

/**
 *
 * @param size
 * @param buffer
 * @return
 */
bool lnSerialBpTxOnlyDma::transmit(int size, const uint8_t *buffer)
{
    return transmitMe(size, buffer, _dmaCallback);
}
/**
 * @brief
 *
 * @param size
 * @param buffer
 * @param c
 * @return true
 * @return false
 */
bool lnSerialBpTxOnlyDma::transmitMe(int size, const uint8_t *buffer, lnDMA::doneCallback *c) //_dmaCallback
{
    // return true;
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    _txMutex.lock();        // lock uart
    _txDma.beginTransfer(); // lock dma
    ENTER_CRITICAL();
    _txState = txTransmittingDMA;
    _programTx();
    d->STAT &= ~LN_USART_STAT_TC;
    _txDma.attachCallback(c, this);
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
// EOF