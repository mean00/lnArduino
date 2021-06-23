#include "lnArduino.h"
#include "lnTimer.h"
#include "lnDAC.h"
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
    
      lnDAC *dac=new lnDAC(0); 
      
      int signalFrequency=30*1000;
      
      dac->startDmaMode(signalFrequency*100);
      int actualFq=dac->getDmaFrequency();
      
    float pointFloat=(float)(actualFq+signalFrequency/2)/(float)signalFrequency;
    int nbPoints=floor(pointFloat);
    Logger("In Fq=%d outFq=%d, # points=%d pointsF=%f\n",signalFrequency,actualFq,nbPoints,pointFloat);
    
    uint16_t xsin[nbPoints];
    for(int i=0;i<nbPoints;i++)
    {
        float angle=2.*M_PI;
        angle/=(float)nbPoints;
        angle*=(float)i;        
        xsin[i]=2048.+2047.*sin(angle);
    }
      
      
    dac->dmaWrite(nbPoints,xsin,true);
    while(1)
    {
        xDelay(10);
    }
      
      dac->doDma(30*1000);
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
