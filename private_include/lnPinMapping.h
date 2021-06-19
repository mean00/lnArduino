
/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

struct LN_PIN_MAPPING
{
    int pin;
    int adc;
    int timer;
    int timerChannel;    
    int dac;
};

extern const LN_PIN_MAPPING pinMappings[];