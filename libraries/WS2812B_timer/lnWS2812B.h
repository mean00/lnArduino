/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

#include "lnSPI.h"
class WS2812B_timer
{
public:
                    WS2812B_timer(int nbLeds, hwlnSPIClass *s);
        virtual     ~WS2812B()_timer;
        
             void   begin(); // call this first
             void   setGlobalBrightness(int value); // between 0 & 255
             void   setColor(int r,int g, int b); // set all the same color
             void   setLedColor(int led, int r,int g, int b); // set only one led
             void   setLedBrightness(int led, int brightness); // set only one led
             void   update(); // call this to have the changes committed

protected:
        int           _pin;
        int           _timer;
        int           _channel;
        int           _nbLeds;
        uint8_t       *_ledsColor;
        uint8_t       *_ledsBrightness;
        uint8_t       _brightness;
protected:
         void         convert(int led);
         void         convertAll();
};
