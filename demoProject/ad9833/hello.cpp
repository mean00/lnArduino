//
//

#include "lnArduino.h"
#include "lnSPI.h"
#include "simplerAD9833.h"


#define AD9833_CS  PB11
#define AD9833_SPI 1

simplerAD9833 *ad;
hwlnSPIClass *spi;

/**
 * 
 */
void setup()
{
    pinMode(LN_SYSTEM_LED,OUTPUT);    
    digitalWrite(LN_SYSTEM_LED,0);
    Logger("Starting AD9833...");
    spi=new hwlnSPIClass(AD9833_SPI);
    spi->begin();
    spi->setSpeed(100*1000); // dont care much about the speed    
    ad=new simplerAD9833(spi,AD9833_CS);
    
}

// M3 @ 112 Mhz Multiply =44, refresh=6 ms
// M4 @ 112 Mhz Multiply =9, refresh=6 ms

/**
 */
volatile float fff=0;
void loop()
{
    Logger("Entering main app...\n");
    int onoff=0;
    
    ad->setWaveForm(simplerAD9833::Square);
    ad->setFrequency(1000);
    ad->enable();
    while(1)
    {
     ad->disable();
     ad->setFrequency(1000);
     ad->enable();
    }
    
    while(1)
    {
        xDelay(1000);
        digitalWrite(LN_SYSTEM_LED,onoff);
        onoff^=1;
    }

}
// EOF
