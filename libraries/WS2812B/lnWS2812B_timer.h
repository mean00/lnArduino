/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnWS2812B.h"
#include "lnSPI.h"
#include "lnTimer.h"
/**
 * 
 * @param nbLeds
 * @param pin
 */
class WS2812B_timer : public WS2812B_base, public lnDmaTimerCallback
{
public:
                    WS2812B_timer(int nbLeds, lnPin pin);
        virtual     ~WS2812B_timer();
        
             void   begin(); // call this first
             void   update(); // call this to have the changes committed
             bool   timerCallback(bool half);

protected:
            lnPin        _pin;
            lnDmaTimer   *_timer;
            uint16_t      _timerPwmValue[48]; 
            uint16_t      _one,_zero;
            int           _nextLed;
            
            void        convertOne(uint8_t value, uint16_t *target);
            void        convertRgb(int hilow, uint8_t *rgb);
            xBinarySemaphore _sem; 
            uint16_t    lookup[16*4];
            
};
