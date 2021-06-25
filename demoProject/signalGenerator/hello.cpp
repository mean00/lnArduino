#include "lnArduino.h"
#include "lnSPI.h"
#include "signal.h"
#include "gd32ST7735.h"
#include "FreeSans7pt7b.h"

#define splash_width 128
#define splash_height 96


// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13



// MOSI is PA7
// CLK  is PA5
// MISO is PA6
// NSS  is PA4

#define PINDC PB0
#define PINCS PB2
#define PINRST PB1


/**
 * 
 */

void setup()
{
    pinMode(LED,OUTPUT);
    digitalWrite(LED,HIGH);
}
/**
 */
void loop()
{
    Logger("Entering main app...\n");
   
#if 1   
    Logger("Starting LCD...\n");
    hwlnSPIClass *spi=new hwlnSPIClass(0,-1);
    spi->begin();
    lnSPISettings transaction(30*1000*1000, SPI_MSBFIRST, SPI_MODE0,-1);
    spi->beginTransaction(transaction);
    // Reset LCD
    pinMode(PINRST,OUTPUT);
    digitalWrite(PINRST,HIGH);
    xDelay(50);
    digitalWrite(PINRST,LOW);
    xDelay(50);
    digitalWrite(PINRST,HIGH);
    
    gd32ST7735 *lcd=new gd32ST7735(160,80,spi,PINDC,PINCS);
    lcd->init();
    lcd->setRotation(2);
    lcd->fillScreen(0);
    
    
    // init fonts
    lcd->setFontFamily(&FreeSans7pt7b,&FreeSans7pt7b,&FreeSans7pt7b);
    lcd->setFontSize(st7735::SmallFont);    
    
    lcd->fillScreen(0);
    lcd->setTextColor(0xFFFF,0);
    lcd->print(2,20,"Signal Generator");
    
#endif    
    Logger("Starting Signal Generator...\n");
    SignalGenerator *signal=new SignalGenerator(PA4,0);    
    signal->start(25000,SignalGenerator::SignalSine);
#if 1    
    Logger("waiting...\n");
    while(1)
    {
        xDelay(10);
    }
#endif
#if 0    
    int r=0;
    int roundup;
    bool onoff;
    while(1)
    {
        roundup++;
        vTaskDelay(1200);
        digitalToggle(LED);
        onoff=!onoff;
        if(onoff)
        {
            lcd->square(0x1f,10,20,40,20);
            lcd->print(20,20,"This is  a test");
        }else
        {
            lcd->square(0xffff,10,20,40,20);
        }
        r++;
        r&=15;
        lcd->setRotation(r>>2);
        Logger("Blue\n");
    }
#endif    
}

