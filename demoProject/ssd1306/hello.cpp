//
#include "Arduino.h"
#include "lnI2C.h"
#include "ssd1306_i2c_lnGd32vf103.h"
// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13
#define SPEED   (200*1000)

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
    
  lnTwoWire *i2c=new lnTwoWire(0,SPEED);
  i2c->begin(0x3c);
  OLED_lnGd32 *ssd1306=new OLED_lnGd32( *i2c, -1);
  ssd1306->begin();
  while(1)
  {
      ssd1306->drawCircle(48,48,25);
      
  }
}
// EOF


