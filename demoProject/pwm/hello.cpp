#include "lnArduino.h"
#include "lnTimer.h"
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



void loop()
{
    bool onoff=true;
    digitalWrite(LED,true);
    int roundup=0;
    
    pinMode(PWM,lnALTERNATE_PP);    
    lnTimer timer(PWM);
    
    
    int ratio=1024;
    int step=128;
    
    timer.setTimerFrequency(50*1000);
    timer.setPwmMode(ratio);
    timer.enable();
    
#if 0    
    int fq=100;
    timer.setPwmMode(512);
    while(1)
    {
        timer.setTimerFrequency(fq);
        Logger("Fq=%d\n",fq);
        fq=fq*10;
        if(fq>=100*1000) fq=100;
        vTaskDelay(100);
        digitalToggle(LED);
        onoff=!onoff;
        
    }
#endif    
    while(1)
    {
        Logger("Ratio=%d\n",ratio);
        timer.setChannelRatio(ratio);
        if(!ratio || ratio==1024) step=-step;
        ratio+=step;
       
        roundup++;
        vTaskDelay(100);
        digitalToggle(LED);
        onoff=!onoff;
        
    }
}
