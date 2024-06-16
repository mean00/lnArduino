/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 *
 *
 */

#pragma once
#include "lnArduino.h"

/**
 *
 * @param instance
 */
class lnSimpleADC 
{
  public:
    lnSimpleADC(int instance, lnPin pin);
    virtual ~lnSimpleADC();
    lnPin pin();
    bool setPin(lnPin pin);
    int simpleRead();                                                 // do a basic sampling
    int getVref();         // direct value
    static float getVcc(); // Vcc value in mv
  protected:
    void readVcc();
    void setup();
    lnPin _pin;
};
// EOF

