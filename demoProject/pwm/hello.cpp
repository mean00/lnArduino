#include "lnArduino.h"

#define LED PA2

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
    while(1)
    {
        roundup++;
        vTaskDelay(500);
        digitalToggle(LED);
        onoff=!onoff;
        Logger("*\n");
    }
}
