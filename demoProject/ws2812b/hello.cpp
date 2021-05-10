#include "lnArduino.h"
#include "lnSPI.h"
#include "lnWS2812B.h"

// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13

#define NB_WS2812B 12

/**
 * 
 */
void setup()
{
    pinMode(LED,OUTPUT);
    digitalWrite(LED,HIGH);
}
/**
 * 
 */
void loop()
{
    // 1-setup WS2812B
   hwlnSPIClass *spi=new hwlnSPIClass(0);
   WS2812B    *ws=new WS2812B(NB_WS2812B,spi);
   ws->begin();
   uint32_t color=0x500a55aaU;
   while(1)
   {
#define B(x) (0xff&(x))       
       ws->setColor(B(color>>16),B(color>>8),B(color));
       color=(color>>31)+(color<<1);
       ws->update();
       xDelay(10);
   }
    
}

