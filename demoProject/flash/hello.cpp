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

const uint8_t schem[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};


void testErase(uint32_t adr)
{
    Logger("Erasing 0x%x\n",adr);
    lnFMC::erase(adr,1);
    uint8_t  *data=(uint8_t *)adr;
    for(int i=0;i<1024;i++)
        if(data[i]!=0xff)
        {
            Logger("Erased failed at offset %d\n",i);
            xAssert(0);
        }
}

void testU(uint32_t adr, const uint8_t *data, int siz)
{
    Logger("Writing %d bytes at address %x \n",siz,adr);
    lnFMC::write(adr,data,siz);
    Logger("Verifying\n");
    uint8_t *ptr=(uint8_t *)adr;
    for(int i=0;i<6;i++)
    {
        if(ptr[i]!=data[i])
        {
            Logger("Error at offset %x\n",i);
            xAssert(0);
        }
    }
}



void loop()
{
    bool onoff=true;
    digitalWrite(LED,true);
    
    uint32_t adr=(uint32_t )&eeprom_begin;
    Logger("Erasing 0x%x\n",eeprom_begin);
    lnFMC::erase(adr,1);
    
    Logger("Writing 6 bytes \n");
    testU(adr,schem,6);

    Logger("Writing 6 bytes , off by one\n");
    lnFMC::erase(adr,1);
    testU(adr+1,schem,6);

    
    Logger("All ok\n");
    while(1)
    {
        __asm__("nop");
    }
}
// EOF