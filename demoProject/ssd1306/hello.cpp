//
#include "Arduino.h"
#include "lnI2C.h"
#include "ssd1306_i2c_lnGd32vf103.h"
// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13
#define SPEED   (400*1000)
#include "FreeSans7pt7b.h"
/**
 * 
 */
void setup()
{
   
}

/**
 * 
 */
void loop()
{
  Logger("SSD1306 I2C demo...\n");
    
  lnI2C *i2c=new lnI2C(0,SPEED);
  i2c->begin(0x3c);
  OLED_lnGd32 *ssd1306=new OLED_lnGd32( *i2c, -1);
  ssd1306->begin();
  ssd1306->setFontFamily(&FreeSans7pt7b,&FreeSans7pt7b,&FreeSans7pt7b);
  ssd1306->setFontSize(OLEDCore::SmallFont);
  while(1)
  {
      Logger("Loop..\n");
      ssd1306->clrScr();
      ssd1306->square(100,20,30,30,true);
      ssd1306->print(2,40,"Demo");
      ssd1306->update();
      xDelay(500);
      ssd1306->clrScr();
      ssd1306->square(2,20,30,30,true);
      ssd1306->print(80,40,"Demo");
      ssd1306->update();
      xDelay(500);
  }
}
// EOF


