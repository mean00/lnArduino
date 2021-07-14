//
// This is a demo using st7735 1.8 inch 128x160 screen
//
//  Color          LCD Pin    MCU PIn
//  Grey           CS         PA4
//  Purple         Reset      PA0
//  Blue           A0/DC      PA12
//  Green          SDA        PA7
//  Brown          SCK        PA5
//

#include "lnArduino.h"
#include "lnSPI.h"
#include "gd32ST7735.h"
#include "FreeSans7pt7b.h"
#include "PNP_compressed.h"
#include "PNP_decl.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160

#define splash_width 128
#define splash_height 96


#define LED LN_SYSTEM_LED

// MOSI is PA7
// CLK  is PA5
// MISO is PA6

#define PINDC  PA12
#define PINCS  PA4
#define PINRST PA0
// 
/**
 * 
 */
void setup()
{
#define WAIT 15
    pinMode(LED,OUTPUT);
    digitalWrite(LED,HIGH);
    
    uint32_t before=millis();
    delayMicroseconds(WAIT*1000);
    uint32_t after=millis();
    Logger("Should be ~ %d : %d, %d delta= %d  \n",WAIT,before,after,(int)(after-before));
    
    int usBefore=micros();
    xDelay(WAIT);
    int usAfter=micros();
    Logger("Should be %d x 1000 : %d->%d,delta=%d  \n",WAIT,usBefore,usAfter,(int)(usAfter-usBefore));
    
}

// M3 @ 112 Mhz Multiply =44, refresh=6 ms
// M4 @ 112 Mhz Multiply =9, refresh=6 ms

/**
 */
volatile float fff=0;
void loop()
{
    Logger("Entering main app...\n");
   
    hwlnSPIClass *spi=new hwlnSPIClass(0,-1);
    spi->begin();
    lnSPISettings transaction(60*1000*1000, SPI_MSBFIRST, SPI_MODE0,-1);
    spi->beginTransaction(transaction);
    // Reset LCD
    pinMode(PINRST,OUTPUT);
    digitalWrite(PINRST,HIGH);
    xDelay(50);
    digitalWrite(PINRST,LOW);
    xDelay(50);
    digitalWrite(PINRST,HIGH);
    
    gd32ST7735 *lcd=new gd32ST7735(SCREEN_WIDTH,SCREEN_HEIGHT,spi,PINDC,PINCS,0,0);
    lcd->init();
    lcd->setRotation(1);
    lcd->fillScreen(0);
    
    { // M3=~ 12, M4=~ 
        int before=lnGetUs();
        fff=3.14;
        for(int i=0;i<100;i++)
        {
            fff=fff*fff;
        }
        int after=lnGetUs();
        Logger("Multiply = %d \n",after-before);       
        xDelay(1000);
    }
#if 1
    while(1)
    {
        lcd->fillScreen(0xff<<6);
        xDelay(1000);
        int before=lnGetUs();
        lcd->fillScreen(0);
        int after=lnGetUs();
        Logger("Refresh = %d \n",after-before);       
        xDelay(1000);
    }
#endif    
    
    lcd->drawRLEBitmap(PNP_width,PNP_height,0,0,0x1f<<6,0x1f<<11,PNP);
    
    // init fonts
    lcd->setFontFamily(&FreeSans7pt7b,&FreeSans7pt7b,&FreeSans7pt7b);
    lcd->setFontSize(st7735::SmallFont);    
    
    
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
    
}
// EOF
