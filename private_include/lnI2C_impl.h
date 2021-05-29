/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"

struct LN_I2C_DESCRIPTOR;


/**
 */
class lnI2CSession 
{
public:
    lnI2CSession(int t, int nb, int *sz, uint8_t **dt)
    {
        target=t;
        nbTransaction=nb;
        transactionSize=sz;
        transactionData=dt;
        curTransaction=0;
        curIndex=0;
    }
    int target;
    int nbTransaction;
    int *transactionSize;
    uint8_t **transactionData;
    int     curTransaction;
    int     curIndex;
};


/**
 * 
 * @param instance
 * @param speed
 */
class lnTwoWire
{
    
public:
                 lnTwoWire(int instance, int speed=0);
                 ~lnTwoWire();
            void setSpeed(int speed);
            void setAddress(int address)
            {
                _targetAddress=address;
            }
            bool write(int n, uint8_t *data)
            {
                return write(_targetAddress,n,data);
            }
            bool read(int n, uint8_t *data)
            {
                return read(_targetAddress,n,data);
            }
            bool write(int target, int n, uint8_t *data);
            bool multiWrite(int target, int nbSeqn,int *seqLength, uint8_t **data);
            bool read(int target,  int n, uint8_t *data);
            bool begin(int target=0);
protected:
            void stopIrq();
            void startIrq();
            bool sendNext();
           
protected:  
    
            enum _I2C_TX
            {
                I2C_TX_START=0,
                I2C_TX_ADDR_SENT=1,
                I2C_TX_DATA=2,
                I2C_TX_STOP=3,
                I2C_TX_END=4,
            };
    
            int _instance;
            int _targetAddress;
            int _speed;
            const LN_I2C_DESCRIPTOR *_d;
            xBinarySemaphore        _sem;
            bool                    _result;
            _I2C_TX                 _txState;
            lnI2CSession            *_session;
public:            
            void irq(int evt);            
            
};