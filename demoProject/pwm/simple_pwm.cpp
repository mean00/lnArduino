#include "lnArduino.h"
#include "lnTimer.h"


#define LED LN_SYSTEM_LED

#define PWM_PIN PA6

void setup()
{
    pinMode(LED,OUTPUT);    
}
int xduration=5;
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
    
    lnPinMode(PWM_PIN,lnPWM);    
    lnTimer timer(PWM_PIN);
    
    Logger("PWM on pin %d\n", PWM_PIN );
    
    timer.setPwmFrequency(1000);
    timer.setPwmMode(50); // 50 % duty cycle
    timer.enable();
    while(1)
    {
        digitalToggle(LED);
        xDelay(1000);
    }
}
// EOF