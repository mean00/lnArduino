#include "lnArduino.h"
#include "lnTimer.h"


#define LED LN_SYSTEM_LED

#define PWM_PIN PA6

void setup()
{
    pinMode(LED,OUTPUT);
    digitalWrite(LED,true);
}


void loop()
{
    lnPinMode(PWM_PIN,lnPWM);    
    lnTimer timer(PWM_PIN);
    
    Logger("PWM on pin %d\n", PWM_PIN );


    timer.setPwmFrequency(1000);
    timer.setPwmMode(50); // 50 % duty cycle
    timer.enable();
    Logger("Shifting frequency\n");
    int fq=1000;

    while(1)
    {
        timer.disable();
        timer.setPwmFrequency(fq);
        timer.enable();
        Logger("Fq=%d\n",fq);
        fq=fq*10;
        if(fq>=100*1000) fq=100;
        xDelay(1000);
        digitalToggle(LED);        
    }
}
