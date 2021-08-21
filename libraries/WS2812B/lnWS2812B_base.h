/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

class WS2812B_base
{
public:
                    WS2812B_base(int nbLeds);
        virtual     ~WS2812B_base();
        
             void   setGlobalBrightness(int value); // between 0 & 255
             void   setColor(int r,int g, int b); // set all the same color
             void   setLedColor(int led, int r,int g, int b); // set only one led
             void   setLedBrightness(int led, int brightness); // set only one led
             virtual void   update()=0; // call this to have the changes committed

protected:
        
        int           _nbLeds;
        uint8_t       *_ledsColor;
        uint8_t       *_ledsBrightness;       
        uint8_t       _brightness;
};