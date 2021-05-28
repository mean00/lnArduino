/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"
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
            
protected:    
        int _instance;
        int _targetAddress;
        int _speed;
};