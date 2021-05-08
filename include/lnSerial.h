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
    lnSerial(int instance);    
    bool init();
    bool setSpeed(int speed);
    bool enableTx(txMode mode);
    bool transmit(int size,uint8_t *buffer);
    bool dmaTransmit(int size,uint8_t *buffer);
    void _interrupt(void);
    
 static void interrupts(int instance);
protected:
    int _instance;
    IRQn_Type _irq;
    bool _stateTx;
    uint32_t _adr;
    xMutex _mutex;
    xBinarySemaphore _txDone;
    uint8_t *_cur,*_tail;
    txState _txState;
    lnDMA   _txDma;
    
protected:    
    void txDmaCb();
 static    void _dmaCallback(void *c);
    
    
};