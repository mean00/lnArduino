#include "lnArduino.h"
#include "lnSPI.h"


// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13

#define NB_WS2812B 12
const uint8_t multiply[2]={0x7<<5,0x1f<<3};
uint8_t ledBuffer[3*8*NB_WS2812B];

hwlnSPIClass *spi;

void flushWs2812B()
{
#if 1
    spi->dmaWrite(NB_WS2812B*3*8,ledBuffer);
#else
    spi->write(NB_WS2812B*3*8,ledBuffer);
#endif
}

void expandColor(int color, uint8_t *p)
{
#if 0
    for(int i=0;i<4;i++)
    {
        p[0]=0;
        p[1]=0x55;
        p+=2;
    }
    return;
#endif
    int mask=1<<7;
    for(int i=0;i<8;i++)
    {
        if(color&mask) *p=multiply[1];
                  else *p=multiply[0];
        p++;
        mask>>=1;
    }
}

void setColor(uint32_t color)
{
    uint8_t r=color&0xff;
    uint8_t g=color>>8;
    uint8_t b=color>>16;
    uint8_t *p=ledBuffer;
    for(int i=0;i<NB_WS2812B;i++)
    {
        expandColor(r,p);
        p+=8;
        expandColor(g,p);
        p+=8;
        expandColor(b,p);
        p+=8;        
    }
}
/**
 * 
 */
void demoMe()
{
   // 1-setup WS2812B
   spi=new hwlnSPIClass(0);
   spi->begin();
   spi->setSpeed(108000000/16);
   spi->setDataMode(SPI_MODE0);
   uint32_t color=0x500a55aaU;
   while(1)
   {
       setColor(color);
       color=(color>>31)+(color<<1);
       flushWs2812B();
       xDelay(10);
   }
    
    
}

/**
 * 
 */
void loop()
{
    Logger("Entering main app...\n");
    pinMode(LED,OUTPUT);
    bool onoff=true;
    digitalWrite(LED,HIGH);
    demoMe();
    
}

