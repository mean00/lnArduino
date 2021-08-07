#include "lnArduino.h"
#include "lnTimer.h"
#include "lnDAC.h"
#include "math.h"

#include "nvm_gd32.h"

#define PWM PB9
#define TIMER 3
#define CHANNEL 3

void setup()
{
    pinMode(LN_SYSTEM_LED,OUTPUT);    
}
/**
 * 
 */


      int val;
void loop()
{
   lnNvmGd32 nvm;
   
   Logger("Erasing..");
   nvm.format();
   
   while(1)
   {
       delay(10);
   }
}
