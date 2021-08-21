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
WS2812B_timer::WS2812B_timer(int nbLeds, int pin)
{
    _nbLeds=nbLeds;
    
        
    _brightness=0xff;
    _ledsColor=new uint8_t[3*nbLeds];
    _ledsBrightness=new uint8_t[nbLeds];
    
    for(int i=0;i<nbLeds;i++)
    {
        _ledsColor[3*i+0]=0;
        _ledsColor[3*i+1]=0;
        _ledsColor[3*i+2]=0;
        _ledsBrightness[i]=255;        
    }
    
}
/**
 * 
 */
void WS2812B_timer::begin()
{
    int div;
}

/**
 */
WS2812B_timer::~WS2812B_timer()
{
#define XCLR(x)     delete [] x;x=NULL;
    XCLR(_ledsBrightness);
    XCLR(_ledsColor);
}
/**
 * 
 * @param value
 */
 void   WS2812B_timer::setGlobalBrightness(int value)
 {
     _brightness=value;
     convertAll();         
 }
 /**
  * 
  * @param r
  * @param g
  * @param b
  */
 void   WS2812B_timer::setColor(int r,int g, int b)
 {
     uint8_t *pr=_ledsColor;
     uint8_t *pg=_ledsColor+1;
     uint8_t *pb=_ledsColor+2;
     for(int i=0;i<_nbLeds;i++)
     {
            *pr=r;
            *pg=g;
            *pb=b;
            pr+=3;pb+=3;pg+=3;
     }
     convertAll();
 }
 /**
  * 
  * @param led
  * @param r
  * @param g
  * @param b
  */
 void   WS2812B_timer::setLedColor(int led, int r,int g, int b)
 {
      uint8_t *p=_ledsColor+led*3;
      p[0]=g;
      p[1]=r;
      p[2]=b;
      convert(led);
 }
 /**
  * 
  * @param led
  * @param brightness
  */
 void   WS2812B_timer::setLedBrightness(int led, int brightness)
 {
     _ledsBrightness[led]=brightness;
     convert(led);     
 }
 /**
  * 
  */
 void WS2812B_timer::update()
 {
  
 }
 

// EOF
 
