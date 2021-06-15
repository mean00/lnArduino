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
    
    lnTimer timer(TIMER,CHANNEL);
    timer.setTimerFrequency(5000);
    timer.setPwmMode(512);
    
    while(1)
    {
        roundup++;
        vTaskDelay(500);
        digitalToggle(LED);
        onoff=!onoff;
        Logger("*\n");
    }
}
