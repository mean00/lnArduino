//
//

#include "libraries/simplerAD9833/simplerAD9833.h"
#include "lnArduino.h"
#include "lnSPI.h"
#include "simplerAD9833.h"

#if defined(USE_RP2040) || defined(USE_RP2350)
#define AD9833_CS GPIO12
#define AD9833_SPI 0
#else
#define AD9833_CS PB11
#define AD9833_SPI 1
#endif

simplerAD9833 *ad;
lnSPI *spi;

void spiLoop();
/**
 *
 */
void setup()
{
    lnPinMode(LN_SYSTEM_LED, lnOUTPUT);
    lnDigitalWrite(LN_SYSTEM_LED, 0);
    Logger("Starting AD9833...");
    spi = lnSPI::create(AD9833_SPI);
    spi->begin();
    spi->setSpeed(1000); // dont care much about the speed
    spi->setBitOrder(SPI_MSBFIRST);
    spi->setDataMode(SPI_MODE2);
    ad = new simplerAD9833(spi, AD9833_CS);
}

void loop()
{
    Logger("Entering main app...\n");
    int onoff = 0;

    ad->setFrequency(1000);
    ad->setWaveForm(simplerAD9833::Square);
    ad->enable();

    while (1)
    {
        ad->setWaveForm(simplerAD9833::Triangle);
        ad->setFrequency(1000);
        lnDelayMs(2000);
        ad->setWaveForm(simplerAD9833::Square);
        ad->setFrequency(10000);
        lnDelayMs(2000);
        ad->disable();
        lnDelayMs(2000);
        ad->enable();
    }
}
// EOF
