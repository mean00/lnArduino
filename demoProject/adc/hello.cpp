#include "lnArduino.h"
#include "lnTimer.h"
#include "lnDAC.h"
#include "lnADC.h"
#include "math.h"
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
    
      
      lnSimpleADC *adc=new lnSimpleADC(0,PA0);
      
      pinMode(PA0,lnADC_MODE);
      
      lnDAC *dac=new lnDAC(0); 
      
   
      dac->simpleMode();
      dac->enable();

      while(1)
      {
          dac->setValue(val);
          val++;
          val&=(1<<13)-1;
          int read=adc->simpleRead();
          Logger("DAC:%d ADC=%d\n",val,read);
          xDelay(1000);
      }
    
}
