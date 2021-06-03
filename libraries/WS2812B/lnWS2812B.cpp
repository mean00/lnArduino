/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 *  Each bit in the WS2812B stream becomes a byte
 * 
 */
#include "lnArduino.h"
#include "lnWS2812B.h"

#define WS_PREAMBLE 4*24 // number of zero byte preamble

/**
 * 
 * @param nbLeds
 * @param s
 */
WS2812B::WS2812B(int nbLeds, hwlnSPIClass *s)
{
    _nbLeds=nbLeds;
    _spi=s;
    _brightness=0xff;
    _ledsColor=new uint8_t[3*nbLeds];
    _ledsBrightness=new uint8_t[nbLeds];
    int up=((nbLeds+3)&(~3))+(WS_PREAMBLE+23)/24; // next multiple of 4 + add one before / one after
    _ledsColorSPI=new uint8_t[3*up*8]; // spi expansion    
    for(int i=0;i<nbLeds;i++)
    {
        _ledsColor[3*i+0]=0;
        _ledsColor[3*i+1]=0;
        _ledsColor[3*i+2]=0;
        _ledsBrightness[i]=255;        
    }
    memset(  _ledsColorSPI,0,3*up*8);
    
}
/**
 * 
 */
void WS2812B::begin()
{
    xAssert(_spi);
    _spi->begin();
    _spi->setSpeed(108000000/16);
    _spi->setDataMode(SPI_MODE1);
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
 void   WS2812B::setLedBrightness(int led, int brightness)
 {
     _ledsBrightness[led]=brightness;
     convert(led);     
 }
 /**
  * 
  */
 void WS2812B::update()
 {
#if 0
     // If need be we change the first byte to lower the duration of the "HIGH" state
     // as the GD32 adds ~ 100 ns of extra high state
     uint8_t v=_ledsColorSPI[0];
     switch(v)
     {
         case (0x1f<<3):
            v=0x1e<<3;
            break;                    
         case (0x7<<5):
            v=0x6<<5;
            break;
         default:
            break;                            
     }
     _ledsColorSPI[0]=v;
#endif     
     _spi->dmaWrite(_nbLeds*3*8+WS_PREAMBLE,_ledsColorSPI); // add a couple of zeros at the end
 }
/**
 * 
 * @param led
 */   
#define ZRANK(x,y)  ((((x*0x1fUL)<<3UL)+(((1UL-x)*0x7UL)<<5UL))<<y)
//#define ZZ(a,b,c,d)   ZRANK(a,24)+ZRANK(b,16)+ZRANK(c,8)+ZRANK(d,0)
 #define ZZ(a,b,c,d)   ZRANK(a,0)+ZRANK(b,8)+ZRANK(c,16)+ZRANK(d,24)
 
 
static const uint32_t   lookupTable[16]=
{ 
            ZZ(0,0,0,0),
            ZZ(0,0,0,1),
            ZZ(0,0,1,0),
            ZZ(0,0,1,1),
            ZZ(0,1,0,0),
            ZZ(0,1,0,1),
            ZZ(0,1,1,0),
            ZZ(0,1,1,1),
            
            ZZ(1,0,0,0),
            ZZ(1,0,0,1),
            ZZ(1,0,1,0),
            ZZ(1,0,1,1),
            ZZ(1,1,0,0),
            ZZ(1,1,0,1),
            ZZ(1,1,1,0),
            ZZ(1,1,1,1)
};
 
static void convertOne(int color, int b16, uint8_t *target)
{
     uint32_t *p=(uint32_t *)target;
     
     color=(color*b16)>>16;
     
     p[0]=lookupTable[(color>>4)&0xf];
     p[1]=lookupTable[(color)&0xf];
}
 /**
  * 
  * @param led
  */
void         WS2812B::convert(int led)
{
    uint8_t *p=_ledsColorSPI+3*8*led+WS_PREAMBLE;
    uint8_t *c=_ledsColor+led*3;
    
    int  b=(int)(_ledsBrightness[led])*(int)(_brightness);
    
    
    convertOne(c[0],b,p);
    convertOne(c[1],b,p+8);
    convertOne(c[2],b,p+16);
  
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
 