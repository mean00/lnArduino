#include "lnArduino.h"
#include "lnTimer.h"

#ifdef __arm__ 
#define LED PC13
#else
#define LED PA2
#endif

#define PWM_PIN PB9
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
#if 1
    Logger("Shifting frequency\n");
    timer.setTimerFrequency(50*1000);
    timer.setPwmMode(ratio);
    timer.enable();
    

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
#if 0
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
#endif
}
