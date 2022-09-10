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
class lnI2C
{    
public:
                         lnI2C(int instance, int speed=0);
        virtual         ~lnI2C();
                    void setSpeed(int speed);
                    void setAddress(int address);
                    bool write(int n, const uint8_t *data)  ;                 
                    bool read(int n, uint8_t *data);
                    bool write(int target, int n, const uint8_t *data);
                    bool multiWrite(int target, int nbSeqn,const int *seqLength, const uint8_t **data);
                    bool read(int target,  int n, uint8_t *data);
                    bool begin(int target=0);       
protected:
                    void *cookie;
            
};
