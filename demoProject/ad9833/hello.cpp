//
//

#include "lnArduino.h"
#include "lnSPI.h"
#include "simplerAD9833.h"
#include "libraries/simplerAD9833/simplerAD9833.h"


#define AD9833_CS  PB11
#define AD9833_SPI 1

simplerAD9833 *ad;
hwlnSPIClass *spi;


void spiLoop();
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
    spi->setSpeed(1000); // dont care much about the speed    
    spi->setBitOrder(SPI_MSBFIRST);
    spi->setDataMode(SPI_MODE2);
    
    pinMode(AD9833_CS,OUTPUT);
    digitalWrite(AD9833_CS,true);
    spiLoop();
    ad=new simplerAD9833(spi,AD9833_CS);    
}

void wr(int x)
{
        digitalWrite(AD9833_CS,false);
        xDelay(1);
        spi->write(x>>8);
        spi->write(x&0xff);
        xDelay(1);
        digitalWrite(AD9833_CS,true);
        xDelay(1);
}
#include "simplerAD9833_internal.h"
void spiLoop()
{
        // Reset
        wr(LN_AD9833_RESET);
        xDelay(1);
        //
        wr(0x00);
#if 0 // this works ~ 5 Mhz        
    while(1)
    {
        wr(LN_AD9833_OPBITEN);
        xDelay(2000);
        wr(LN_AD9833_OPBITEN+LN_AD9833_DIV2);
        xDelay(2000);
        wr(0);
        xDelay(2000);
       // wr(LN_AD9833_HLB); // Wrote
       // wr(LN_AD9833_CTRL(1)+0xf00); // Wrote
        
        //xDelay(2000);
        
    }
#endif   
    // ~ 100 hz
#define CMD LN_AD9833_OPBITEN*0
    while(1)
    {
        //wr(CMD);       
        //xDelay(1);
        wr(LN_AD9833_B28+CMD); // Wrote
        wr(LN_AD9833_FREQ+0xf00); // Wrote
        wr(LN_AD9833_FREQ+0x000); // Wrote
        
        xDelay(2000);
        
    }
        
        
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
    
    ad->setWaveForm(simplerAD9833::Triangle);
    //ad->setFrequency(1000);
    ad->enable();
    //ad->setWaveForm()
    while(1)
    {
        xDelay(1000);
    }
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
