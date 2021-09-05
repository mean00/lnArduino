//
//

#include "lnArduino.h"
#include "lnSPI.h"
#include "simplerAD8933.h"


#define AD8933_CS  PB11
#define AD8933_SPI 1

simplerAD8933 *ad;
hwlnSPIClass *spi;

/**
 * 
 */
void setup()
{
    pinMode(LN_SYSTEM_LED,OUTPUT);    
    digitalWrite(LN_SYSTEM_LED,0);
    Logger("Starting AD8933...");
    spi=new hwlnSPIClass(AD8933_SPI);
    spi->begin();
    spi->setSpeed(100*1000); // dont care much about the speed    
    ad=new simplerAD8933(spi,AD8933_CS);
    
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
    while(1)
    {
        xDelay(1000);
        digitalWrite(LN_SYSTEM_LED,onoff);
        onoff^=1;
    }

}
// EOF
