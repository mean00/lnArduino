#include "lnArduino.h"


#ifdef __arm__ 
#define LED PC13
#else
#define LED PA2
#endif

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
    
    while(1)
    {
        roundup++;
        vTaskDelay(500);
        digitalToggle(LED);
        onoff=!onoff;
      //  Logger("*\n");
    }
}
