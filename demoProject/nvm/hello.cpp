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

#define TAIST(x) {if(!(x)) Logger("FAILURE of %s at line \n",#x,__LINE__);return;}

      int val;
      
void InitNvm(lnNvmGd32 &nvm)
{
    
    uint8_t v=1;
   TAIST(nvm.write(1,1,&v));
   v=0;
   
   nvm.dumpEntries(0);
   
   TAIST(nvm.read(1,1,&v));
   TAIST(v==1);

   v=2;
   TAIST(nvm.write(1,1,&v));
   nvm.dumpEntries(0);
   v=0;
   TAIST(nvm.read(1,1,&v));
   TAIST(v==2);

   v=3;
   TAIST(nvm.write(1,1,&v));
   nvm.dumpEntries(0);
   v=0;
   TAIST(nvm.read(1,1,&v));
   TAIST(v==3);

}

void TestNvm()
{
    uint8_t v;
    lnNvmGd32 nvm;
    TAIST(nvm.begin());
    TAIST(nvm.read(1,1,&v));
    TAIST(v==3);
}
   int roundx;   
void loop()
{
   lnNvmGd32 nvm;
   //nvm.format();
   //delay(5000);
   if(!nvm.begin())
   {
       nvm.format();
   }
   for(int roundx=0;roundx<40;roundx++)
   {
        InitNvm(nvm);
   }

 
   
   Logger("End\n");
   while(1)
   {
       delay(10);
   }
}
