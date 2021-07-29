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
 */
class lnBaseAdc
{
public:
                    lnBaseAdc(int instance)
                    {
                        _instance=instance;
                    }
                    ~lnBaseAdc()
                    {

                    }
            int      getVref(); // direct value
            float    getVcc();  // Vcc value in mv
protected:
            void     readVcc();
    virtual void     setup();
    static int       adcChannel(lnPin pin);
            int      _instance;
};

/**
 * 
 * @param instance
 */
class lnSimpleADC : public lnBaseAdc
{
public:
            lnSimpleADC(int instance, lnPin pin);
   virtual ~lnSimpleADC();
   lnPin   pin();
   bool    setPin(lnPin pin);
   int     simpleRead();      // do a basic sampling
   bool    pollingMultiRead(int nbPins, lnPin *pins, int *output); // read N pins in one go POLLING
   
   int      getVref(); // direct value
   float    getVcc();  // Vcc value in mv
protected:
    void    readVcc();
    void    setup();
    int     _instance;
    int     _pin;

};
/**
 * 
 * @param instance
 */
class lnTimingAdc
{
public:
            lnTimingAdc(int instance);
   virtual  ~lnTimingAdc();
   bool     multiRead(int fq, int nbSamplePerChannel, int nbPins, lnPin *pins, int *output); // read N pins in one go POLLING
   
   int      getVref(); // direct value
   float    getVcc();  // Vcc value in mv
protected:
    void    readVcc();
    void    setup();
    int     _instance;
};
// EOF