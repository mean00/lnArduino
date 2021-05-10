#include "lnArduino.h"
#include "lnSPI.h"
#include "lnWS2812B.h"

// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13

#define NB_WS2812B 12

/**
 * 
 */
void setup()
{
    pinMode(LED,OUTPUT);
    digitalWrite(LED,HIGH);
}

// Derived from the WS2812Demo found in stm32duino
//
void Wheel(byte WheelPos, int &r, int &g, int &b) 
{
    if(WheelPos < 85) 
    {
      r=WheelPos * 3;
      b=255 - WheelPos * 3;
      g=0;
      return;
    } 
    if(WheelPos < 170) 
    {
     WheelPos -= 85;
     r=255 - WheelPos * 3;
     g=0;
     b=WheelPos * 3;
     return;
    } 
    WheelPos -= 170;
    r=0;
    g=WheelPos * 3;
    b=255 - WheelPos * 3; 
}

/**
 * 
 */
void loop()
{
    // 1-setup WS2812B
   hwlnSPIClass *spi=new hwlnSPIClass(0);
   WS2812B    *ws=new WS2812B(NB_WS2812B,spi);
   ws->begin();
   int r,g,b;
   while(1)
   {
     for(int j=0; j<256; j++) 
     {
        for(int i=0; i<NB_WS2812B; i++) 
        {
         int pos=(i+j)&0xff;
         Wheel(pos,r,g,b);
         ws->setLedColor(i,r,g,b);
        }
        ws->update();
        xDelay(10);
     }
  }
}
// EOF

