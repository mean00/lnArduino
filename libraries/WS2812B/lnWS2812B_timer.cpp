/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 *  Each bit in the WS2812B_timer stream becomes a byte
 * 
 */
#include "lnArduino.h"
#include "lnWS2812B_timer.h"


#define WS_PREAMBLE 4*24 // number of zero byte preamble

/**
 * 
 * @param nbLeds
 * @param s
 */
WS2812B_timer::WS2812B_timer(int nbLeds, int pin) : WS2812B_base(nbLeds)
{
   _timer=new lnDmaTimer(pin);
   
    
}
/**
 * 
 */
void WS2812B_timer::begin()
{
   if(!_timer->pwmSetup(5000000))  // at least 6 ticks @ 0.83 Mhz =>  ~ 5 Mhz       
   {
       xAssert(0);
   }
   _one=(_timer->rollover()*2)/6;
   _zero=(_timer->rollover()*4)/6;   
   _timer->attachDmaCallback(this);
}

/**
 */
WS2812B_timer::~WS2812B_timer()
{
}
void WS2812B_timer::convertOne(uint8_t value, uint16_t *target)
{
    for(int i=0;i<8;i++)
    {
        if(value & 1) *target=_one;
        else *target=_zero;
        target++;
        value>>=1;
    }
}
/**
 * 
 * @param hilow
 * @param rgb
 */
void WS2812B_timer::convertRgb(int hilow, uint8_t *rgb)
{
    uint16_t *p=_timerPwmValue;
    if(hilow) p+=24;
    convertOne(rgb[0],p);
    convertOne(rgb[1],p+8);
    convertOne(rgb[2],p+16);
}

 /**
  * 
  */
 void WS2812B_timer::update()
 {
    _nextLed=2;
    convertRgb(false,_ledsColor);
    convertRgb(true,_ledsColor+3);
    // start PWM 
 }
/**
 * 
 * @param half
 * @return 
 */
bool   WS2812B_timer::timerCallback(bool half)
{
    if(_nextLed==_nbLeds) return false; // done!
    convertRgb(!half,_ledsColor+3*_nextLed);
    _nextLed++;
    return true;    
}
// EOF
 
