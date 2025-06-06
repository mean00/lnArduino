#include "esprit.h"
#include "lnExti.h"
#define LED PA2
#define BOOT0 PA8

lnPin pin = NoPin;
lnBinarySemaphore *sem;

void button0Cb(lnPin apin, void *cookie)
{
    pin = apin;
    sem->give();
    lnExtiDisableInterrupt(BOOT0);
}

void setup()
{
    lnPinMode(LED, lnOUTPUT);
    sem = new lnBinarySemaphore;
    lnExtiAttachInterrupt(BOOT0, LN_EDGE_BOTH, button0Cb, NULL);
}
/**
 *
 */
void loop()
{
    lnExtiEnableInterrupt(BOOT0);
    while (1)
    {
        sem->take();
        Logger("Got interrupt %d (%d)!\n", pin, BOOT0);
        pin = NoPin;
        lnExtiEnableInterrupt(BOOT0);
    }
}
