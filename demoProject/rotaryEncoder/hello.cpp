#include "lnArduino.h"
#include "lnExti.h"
#include "RotaryEncoder.h"


#ifdef __arm__ 
    #define LED PC13
#else
    #define LED PA2
#endif

lnPin pin=-1;
xBinarySemaphore *sem;


void setup()
{
    pinMode(LED,OUTPUT);
   
}
/**
 * 
 */
lnRotary *rotary;
void loop()
{
    rotary=new lnRotary(PB5,PB6,PB7);
    rotary->start();
    int val=0;
    while(1)
    {
        lnRotary::EVENTS  ev=rotary->waitForEvent(2000);
        Logger("Pulse\n");
        digitalToggle(LED);
        
        if(ev& lnRotary::ROTARY_CHANGE)
        {
            val+=rotary->getCount();
            Logger("New value =%d\n",val);
        }
        if(ev & lnRotary::SHORT_PRESS)
        {
            Logger("Short Press\n");
        }
        if(ev & lnRotary::LONG_PRESS)
        {
            Logger("Long Press\n");
        }        
    }
}
