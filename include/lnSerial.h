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
    bool transmit(int size,uint8_t *buffer);
    bool dmaTransmit(int size,uint8_t *buffer);
    void purgeRx();
    void _interrupt(void);

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
    uint8_t *_cur,*_tail;
    txState _txState;
    lnDMA   _txDma;
    int     _rxBufferSize;
    int     _rxHead,_rxTail;
    uint8_t *_rxBuffer;
    xMutex   _rxMutex;

protected:
    void txDmaCb();
 static    void _dmaCallback(void *c,lnDMA::DmaInterruptType it);


};
