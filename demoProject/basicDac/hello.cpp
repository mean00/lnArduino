#include "lnArduino.h"
#include "lnTimer.h"
#include "lnDAC.h"
#define LED PA2
#define PWM PB9
#define TIMER 3
#define CHANNEL 3

void setup()
{
    pinMode(LED,OUTPUT);
    
}
/**
 * 
 */


      int val;
void loop()
{
    bool onoff=true;
    digitalWrite(LED,true);
    int roundup=0;

//#define mode   LN_CTL_INPUT_ANALOG
#define mode  lnDAC_MODE
    
      pinMode(PA4,mode);
      pinMode(PA5,mode);
    
      lnDAC *dac=new lnDAC(0); 
      
      dac->startDmaMode();
      
      
      dac->doDma(100000);
      xAssert(0);
      dac->simpleMode();
      dac->enable();

      while(1)
      {
          dac->setValue(val);
          val++;
          val&=(1<<13)-1;
      }
    
}
