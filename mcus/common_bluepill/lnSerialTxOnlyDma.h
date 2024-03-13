#pragma once
#include "lnDma.h"
#include "lnRingBuffer.h"

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
    virtual bool _programTx();
    static void _dmaCallback(void *c, lnDMA::DmaInterruptType it);
    void txDmaCb(lnDMA::DmaInterruptType it);
    lnDMA _txDma;
};
/**
 * @brief
 *
 */
class lnSerialBpTxOnlyBufferedDma : public lnSerialBpCore, public lnSerialTxOnly
{
  public:
    lnSerialBpTxOnlyBufferedDma(int instance, int txBufferSize);
    virtual ~lnSerialBpTxOnlyBufferedDma();
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
    virtual bool _programTx();
    static void _dmaCallback2(void *c, lnDMA::DmaInterruptType it);
    void txDmaCb2(lnDMA::DmaInterruptType it);
    void igniteTx();
    lnDMA _txDma;

    lnRingBuffer _txRingBuffer;
    lnBinarySemaphore _txRingSem;
    bool _txing;
    int _inFlight;
};

// EOF