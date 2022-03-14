/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
#include "lnArduino.h"
#include "lnDma.h"
/**
 */



class lnSerial
{
public:
    enum Event
    {
      dataAvailable,
    };
    enum txState
    {
        txIdle,
        txTransmitting,
        txLast
    };
    enum txMode
    {
        txNone,
        txInterrupt,
        txDma
    };
         lnSerial(int instance, int rxBufferSize=128);
    bool init();
    bool setSpeed(int speed);
    bool enableRx(bool enabled);
    bool transmit(int size,const uint8_t *buffer);
    bool dmaTransmit(int size,const uint8_t *buffer);
    void disableInterrupt();
    void enableInterrupt();
    void purgeRx();
    void _interrupt(void);
    int   read(int max, uint8_t *to);
typedef void lnSerialCallback(void *cookie, lnSerial::Event event);
    void setCallback(lnSerialCallback *cb, void *cookie)
            {
              _cb=cb;
              _cbCookie=cookie;
            }
 static void interrupts(int instance);
protected:
    void txInterruptHandler(void);
    void rxInterruptHandler(void);

protected:
    bool _enableTx(txMode mode);
    int _instance;
    LnIRQ _irq;
    bool _stateTx;
    uint32_t _adr;
    xMutex _mutex;
    xBinarySemaphore _txDone;
    volatile const uint8_t *_cur,*_tail;
    txState _txState;
    lnDMA   _txDma;
    int     _rxBufferSize;
    int     _rxHead,_rxTail;
    uint8_t *_rxBuffer;
    int     modulo(int in);
    lnSerialCallback   *_cb;
    void               *_cbCookie;

protected:
    void txDmaCb();
 static    void _dmaCallback(void *c,lnDMA::DmaInterruptType it);


};
