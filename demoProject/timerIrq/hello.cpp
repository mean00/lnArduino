#include "lnArduino.h"
#include "lnBasicTimer.h"


#define LED LN_SYSTEM_LED

#define PWM_PIN PA7

void setup()
{
   
}
int xduration=5;

lnBasicDelayTimer *timer=NULL;
xBinarySemaphore *sem=NULL;

void handler(void *cookie)
{
    xBinarySemaphore *s=(xBinarySemaphore *)cookie;
    s->give();
}



void loop()
{
    timer=new lnBasicDelayTimer(0);
    sem=new xBinarySemaphore;
    timer->setInterrupt(handler,sem);
   
    while(1)
    {
        uint32_t now=lnGetUs();
        timer->arm(500);
        sem->take();
        int delta=lnGetUs()-now;
        delta-=500;
        if(delta<0) delta=-delta;
        Logger("Error = %d\n",delta);
        xDelay(300);
    }
    
}
