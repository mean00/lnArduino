/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 * 
 *  
 */

#pragma once
#include "lnDma.h"
class lnBasicTimer;
/**
 * 
 * @param instance
 */
class lnSimpleADC
{
public:
            lnSimpleADC(int instance, lnPin pin);
   virtual ~lnSimpleADC();
   lnPin   pin();
   bool    setPin(lnPin pin);
   int     simpleRead();      // do a basic sampling
   bool    multiRead(int nbPins, lnPin *pins, int *output); // read N pins in one go
   
   int      getVref(); // direct value
   float    getVcc();  // Vcc value in mv
protected:
    void    readVcc();
    void    setup();
    int     _instance;
    int     _pin;

};
// EOF