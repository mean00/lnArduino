#include "lnArduino.h"
#include "lnTimer.h"

#ifdef __arm__ 
#define LED PC13
#else
#define LED PA2
#endif

#define PWM_PIN PB1
#define TIMER 2
#define CHANNEL 3

void setup()
{
    pinMode(LED,OUTPUT);
    
}
/**
 * 
 */

void pulse(lnTimer *t, int duration)
{
        Logger("duration=%d\n",duration);
        lnPinMode(PWM_PIN,lnALTERNATE_PP);   
        t->singleShot(duration); // off by 2%
        digitalWrite(PWM_PIN,0);
        lnPinMode(PWM_PIN,lnOUTPUT);
    
}


void loop()
{
    bool onoff=true;
    digitalWrite(LED,true);
    int roundup=0;
    
    lnPinMode(PWM_PIN,lnALTERNATE_PP);    
    lnTimer timer(PWM_PIN);
    
    Logger("PWM on pin %d\n", PWM_PIN );
    int ratio=1024;
    int step=128;
    
#if 0    
    while(1)
    {
    
        timer.singleShot(1,false);
        digitalToggle(LED);
        xDelay(1000);
    }
#endif
#if 0
    Logger("Shifting frequency\n");
    timer.setTimerFrequency(50*1000);
    timer.setPwmMode(ratio);
    timer.enable();
    

    int fq=1000;
    timer.setPwmMode(512);
    while(1)
    {
        timer.disable();
        timer.setTimerFrequency(fq);
        timer.enable();
        Logger("Fq=%d\n",fq);
        fq=fq*10;
        if(fq>=100*1000) fq=100;
        vTaskDelay(100);
        digitalToggle(LED);
        onoff=!onoff;
        
    }
#endif 


#if 1
    timer.setTimerFrequency(8);
    timer.disable();
    int duration=5;
    while(1)
    {
        
        Logger("duration=%d\n",duration);
        pulse(&timer,duration);
        //duration+=10;
        if(duration>100) duration-=100;
        delay(200);
        digitalToggle(LED);
        
    }
#endif
 
#if 0
    timer.setTimerFrequency(8);
    timer.enable();
    while(1)
    {
        Logger("Ratio=%d\n",ratio);
        timer.setPwmMode(ratio);
        if(!ratio || ratio==1024) step=-step;
        ratio+=step;
       
        roundup++;
        vTaskDelay(100);
        digitalToggle(LED);
        onoff=!onoff;
        
    }
#endif
}
