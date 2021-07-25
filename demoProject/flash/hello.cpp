#include "lnArduino.h"
#include "lnFMC.h"

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

extern int eeprom_begin;

void loop()
{
    bool onoff=true;
    digitalWrite(LED,true);
    
    uint32_t adr=(uint32_t )&eeprom_begin;
    Logger("Erasing 0x%x\n",eeprom_begin);
    lnFMC::erase(adr,1);
    const uint8_t schem[]={0x11,0x22,0x33,0x44,0x55,0x66};
    lnFMC::write(adr,schem,6);
    Logger("All ok\n");
    while(1)
    {
        __asm__("nop");
    }
}
// EOF