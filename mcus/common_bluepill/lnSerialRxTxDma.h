
/**
 * @brief This is to avoid non virtual thunk gdb does not like that
 *
 */
class lnSerialBpRxTxDma;
/**
 * @brief
 *
 */
class timerLink : public lnPeriodicTimer
{
  public:
    timerLink(lnSerialBpRxTxDma *parent) : lnPeriodicTimer()
    {
        _parent = parent;
    }    
    void timerCallback();
protected:    
    lnSerialBpRxTxDma *_parent;
};
/**
 * @brief
 *
 */
class lnSerialBpRxTxDma : public lnSerialBpTxOnlyBufferedDma, public lnSerialRxTx
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
    bool transmit(int size, const uint8_t *buffer)
    {
        return lnSerialBpTxOnlyBufferedDma::transmit(size, buffer);
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
    uint32_t _rxBufferSize;
    uint32_t _rxMask = 0;
    uint32_t _rxHead, _rxTail;
    bool _rxEnabled;
    lnDMA _rxDma;
    uint8_t *_rxBuffer;
};
