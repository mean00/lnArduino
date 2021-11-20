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
    Logger("Go!\n"); 
    while(1)
    {
        for(int del=500;del<1000;del+=100)
            for(int i=0;i<5;i++)
            {
                uint32_t now=lnGetUs();
                timer->arm(del);
                sem->take();
                int delta=lnGetUs()-now;
                delta-=del;
                if(delta<0) delta=-delta;
                Logger("%d Error = %d (now=%d)\n",del,delta,now&0xffff);
                xDelay(300);
            }
    }
    
}
