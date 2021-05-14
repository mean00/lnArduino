/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */


#pragma once
#include "lnPeripherals.h"

class lnPeripherals
{
public:
   
public:            
    static void reset(const Peripherals periph);
    static void enable(const Peripherals periph);
    static void disable(const Peripherals periph);
    static uint32_t getClock(const Peripherals periph);
};


