/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 * 
 * 
 * In simpleMode the setup is as follows :
 * simpleMode();
 * enable();
 * setValue(xxx);
 * setValue(xxx);
 * 
 */

#pragma once
#include "lnDma.h"
class lnBasicTimer;
/**
 * 
 * @param instance
 */
class lnDAC
{
public:
            lnDAC(int instance);
   virtual ~lnDAC();
   lnPin   pin();
   void    enable();
   void    disable();
   void    simpleMode(); // call this if you just want to manually set the dac value
   void    setValue(int value); // 12 bits value 00..495
   
   
   void    startDmaMode();
   void    stopDmaMode();
   void    doDma(int fq);
        
protected:
        int _instance;
        lnDMA _dma;
        lnBasicTimer *_timer;
};