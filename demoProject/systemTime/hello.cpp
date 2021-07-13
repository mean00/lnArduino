#include "lnArduino.h"


#define LED LN_SYSTEM_LED

void setup()
{
    pinMode(LED,OUTPUT);
    
}
/**
 * 
 */
int roundup=0;
void loop()
{
    bool onoff=true;
    digitalWrite(LED,true);
 
    Logger("Starting a 5 sec wait...\n");
    xDelay(5000);
    Logger("End of 5 sec wait\n");
    int before=lnGetUs();
    xDelay(100);
    int after=lnGetUs();
    Logger("Before=%d \n",before);
    Logger("After=%d \n",after);
    Logger("delta=%d , should be 100 000 \n",after-before);
    
    while(1)
    {
        roundup++;
        xDelay(500);
        digitalToggle(LED);
        onoff=!onoff;   
    }
}
