/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnWS2812B.h"

/**
 * 
 * @param nbLeds
 * @param s
 */
WS2812B::WS2812B(int nbLeds, hwlnSPIClass *s)
{
    _nbLeds=nbLeds;
    _spi=s;
    _ledsColor=new uint8_t[3*nbLeds];
    _ledsBrightness=new uint8_t[nbLeds];
    _ledsColorSPI=new uint8_t[3*nbLeds*8]; // spi expansion
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
void WS2812B::begin()
{
    xAssert(_spi);
    _spi->begin();
    _spi->setSpeed(108000000/16);
    _spi->setDataMode(SPI_MODE0);
}

/**
 */
WS2812B::~WS2812B()
{
#define XCLR(x)     delete [] x;x=NULL;
    XCLR(_ledsColorSPI);
    XCLR(_ledsBrightness);
    XCLR(_ledsColor);
}
/**
 * 
 * @param value
 */
 void   WS2812B::setGlobalBrightness(int value)
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
 void   WS2812B::setColor(int r,int g, int b)
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
 void   WS2812B::setLedColor(int led, int r,int g, int b)
 {
      uint8_t *pr=_ledsColor+led*3;
      pr[0]=r;
      pr[0]=g;
      pr[0]=b;
      convert(led);
 }
 /**
  * 
  * @param led
  * @param brightness
  */
 void   WS2812B::setLedBrighness(int led, int brightness)
 {
     _ledsBrightness[led]=brightness;
     convert(led);     
 }
 /**
  * 
  */
 void WS2812B::update()
 {
     _spi->dmaWrite(_nbLeds*3*8,_ledsColorSPI);
 }
/**
 * 
 * @param led
 */   
 const uint8_t multiply[2]={0x7<<5,0x1f<<3};
 static void convertOne(int color, uint8_t *target)
 {
    int mask=1<<7;
    for(int i=0;i<8;i++)
    {
        if(color&mask) *target=multiply[1];
                  else *target=multiply[0];
        target++;
        mask>>=1;
    }
 }
 /**
  * 
  * @param led
  */
void         WS2812B::convert(int led)
{
    uint8_t *p=_ledsColorSPI+3*8*led;
    uint8_t *c=_ledsColor+led*3;
    convertOne(c[0],p);
    convertOne(c[1],p+8);
    convertOne(c[2],p+16);
  
}
/**
 * 
 */
void         WS2812B::convertAll()
{
    for(int i=0;i<_nbLeds;i++)
        convert(i);
}

// EOF
 