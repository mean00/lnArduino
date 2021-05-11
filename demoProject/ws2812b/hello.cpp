//
// The "wheel" effect is derived from the WS2812Demo found in stm32duino
// https://github.com/rogerclarkmelbourne/WS2812B_STM32_Libmaple/blob/master/examples/WS2812B_test/WS2812B_test.ino
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

void wheelDemo(WS2812B *ws)
{
    int r,g,b;
   int before,after;
   
   int brightness=255;
   int dir=-5;
   
   while(1)
   {
     
     for(int j=0; j<256; j++) 
     {
        before=micros();
        for(int i=0; i<NB_WS2812B; i++) 
        {
         int pos=(i+j)&0xff;
         Wheel(pos,r,g,b);
         ws->setLedColor(i,r,g,b);
        }
        after=micros();
        //Logger("Convert = %d\n",after-before);
        before=micros();
        
        after=micros();
        //Logger("Convert2 = %d\n",after-before);
        xDelay(5);
        ws->setGlobalBrightness(brightness);
        brightness+=dir;
        if(brightness==0 && dir<0) dir=-dir;
        if(brightness==255 && dir>0) dir=-dir;
        ws->update();
     }
  }
}
void pulse(WS2812B *ws)
{
    int brightness=0xff;
    int dir=-1;
    ws->setColor(0xff,0,0); // RED G* R* B*
    while(1)
    {
     
     
        ws->setGlobalBrightness(brightness);
        brightness+=dir;
        if(brightness==0 && dir<0) dir=-dir;
        if(brightness==255 && dir>0) dir=-dir;
        ws->update();
        xDelay(10);
    }
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
   
   //wheelDemo(ws);
   pulse(ws);
   
}
// EOF

