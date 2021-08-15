/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

struct LN_PIN_MAPPING
{
    int8_t pin;
    int8_t adc;
    int8_t timer;
    int8_t timerChannel;    
    int8_t dac;
    int8_t filler;
};

extern const LN_PIN_MAPPING pinMappings[];