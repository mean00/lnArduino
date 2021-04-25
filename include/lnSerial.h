#pragma once
#include "lnArduino.h"
/**
 */
class lnSerial
{
public:
    lnSerial(int instance, IRQn_Type irq,uint32_t adr);    
    bool init();
    bool setSpeed(int speed);
    bool enableTx(bool onoff);
    bool transmit(int size,uint8_t *buffer);
protected:
    int _instance;
    IRQn_Type _irq;
    bool _stateTx;
    uint32_t _adr;
    xMutex _mutex;
    xBinarySemaphore _txDone;
    
};