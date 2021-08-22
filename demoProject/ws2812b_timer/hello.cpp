//
// The "wheel" effect is derived from the WS2812Demo found in stm32duino
// https://github.com/rogerclarkmelbourne/WS2812B_STM32_Libmaple/blob/master/examples/WS2812B_test/WS2812B_test.ino
#include "Arduino.h"
#include "lnSPI.h"
#include "lnWS2812B_timer.h"

// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13

#define NB_WS2812B   12
#define WS2812B_PIN  PA6

/**
 * 
 */
void setup()
{
    pinMode(LED,OUTPUT);
    lnPinMode(WS2812B_PIN,lnPWM);
    digitalWrite(LED,true);
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

void wheelDemo(WS2812B_timer *ws)
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
        //xDelay(20);
        //ws->setGlobalBrightness(brightness);
        brightness+=dir;
        if(brightness==0 && dir<0) dir=-dir;
        if(brightness==255 && dir>0) dir=-dir;
        ws->update();
        xDelay(1);
     }
  }
}
void pulse(WS2812B_timer *ws)
{
    int brightness=127;
    int dir=-1;
    ws->setColor(0xff,0,0); // RED G* R* B*
    while(1)
    {
     
     
        ws->setGlobalBrightness(brightness);
        brightness+=dir;
        if(brightness==0 && dir<0) dir=-dir;
        if(brightness==127 && dir>0) dir=-dir;
        ws->update();
        //xDelay(10);
    }
}
#define VAL 0x0f
uint8_t single[3][3]={{0,0,VAL},{0,VAL,0},{VAL,0,0}};
void dot(WS2812B_timer *ws)
{
    int dex=0,olddex=0;
    int r,g,b;
    int color=0;
    uint8_t *start=&(single[0][0]);
    for(int i=0;i<NB_WS2812B;i++)
    {
        uint8_t *t=start+(i%3)*3;
        
        ws->setLedColor(i,t[0],t[1],t[2]);// g r b
        ws->setLedBrightness(i,(255*((i%3)+1))/4);
    }
     ws->update();
     while(1) {}
         
    while(1)
    {
        //ws->setLedBrightness(olddex,10);
        dex++;
        //if(dex>=NB_WS2812B) dex=0;
        //ws->setLedBrightness(dex,255);
        ws->setGlobalBrightness(dex&0xff);
        olddex=dex;
        ws->update();
        //xDelay(80);
    }
}
void singleValue(WS2812B_timer *ws, int v)
{
    Logger("Value =%d\n",v);
    ws->setColor(v,0,0);// r g b
    ws->update();
}

void colors(WS2812B_timer *ws)
{
    
    int dex=0;
    while(1)
    {
        //ws->setLedBrightness(olddex,10);
        dex+=1;
        if(dex>=127) dex=0;        
        ws->setColor(dex,0,0);// r g b
        ws->update();
        Logger("Value =%d\n",dex);
        //xDelay(20);
    }
}
    

void wsDemoWheel()
{
   Logger("Wheel Demo\n");
   WS2812B_timer    *ws=new WS2812B_timer(NB_WS2812B,WS2812B_PIN);
   ws->begin();     
   wheelDemo(ws);
}

void wsSimple()
{
   Logger("wsSimple Demo\n");
   WS2812B_timer    *ws=new WS2812B_timer(NB_WS2812B,WS2812B_PIN);
   ws->begin();
   // simple
   ws->setLedColor(0,0xff,0,0); 
   ws->setLedColor(1,0,0xff,0);
   ws->setLedColor(2,0,0,0xff);
   ws->update();
   while(1)
   {
       xDelay(10);
   }
   
}


void wsAll()
{
   Logger("wsSimple Demo\n");
   WS2812B_timer    *ws=new WS2812B_timer(NB_WS2812B,WS2812B_PIN);
   ws->begin();
   // simple
   for(int i=0;i<NB_WS2812B;i++)
   {
       switch(i%3)
       {
           case 0:ws->setLedColor(i,0xff,0,0);break;  // G R B
           case 1:ws->setLedColor(i,0,0xff,0);break;  // G R B
           case 2:ws->setLedColor(i,0,0,0xff);break;  // G R B
       }
   }
   //ws->setColor(0,0,0xff);  // G R B
   ws->update();
   while(1)
   {
       ws->update();
       xDelay(10);
   }
   
}

int cbCalled=0;
lnDmaTimer *timer;
class timCB : public  lnDmaTimerCallback
{
public:
        virtual bool timerCallback(bool half); // return true if we continue, zero if we stop
};

 bool timCB::timerCallback(bool half)
 {
     cbCalled++;
     return false;
 }

void manualDemo()
{
    timer=new lnDmaTimer(WS2812B_PIN);
    
   if(!timer->pwmSetup(830000)) 
   {
       xAssert(0);
   }
   int one=(timer->rollover()*4+3)/6;
   int zero=(timer->rollover()*2+3)/6;   
   timCB cb;
   timer->attachDmaCallback(&cb);
   uint16_t pwm[24*2];
   for(int i=0;i<48;i++) pwm[i]=zero;
#if 1
   for(int i=0;i<8;i++) pwm[i]=one;    // g
   for(int i=0;i<8;i++) pwm[i+8]=zero; //r
   for(int i=0;i<8;i++) pwm[i+16]=zero; //b 
#endif   
   timer->start(24*2,pwm);
   while(1)
   {
       xDelay(100);
   }
}

/**
 * 
 */
void loop()
{
    Logger("WS2812B_timer demo...\n");
    
    //manualDemo();
    //wsAll();
    wsDemoWheel(); 
    wsSimple();

}
// EOF


