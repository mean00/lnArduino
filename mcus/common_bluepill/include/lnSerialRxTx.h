#pragma once
#include "lnSerialBpCore.h"
#include "lnSerialTxOnly.h"
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
    virtual int transmitNoBlock(int size, const uint8_t *buffer)
    {
        return lnSerialBpTxOnlyInterrupt::transmitNoBlock(size, buffer);
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
    void setCallback(lnSerialCallback *cb, void *cookie)
    {
        lnSerialBpCore::_setCallback(cb, cookie);
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