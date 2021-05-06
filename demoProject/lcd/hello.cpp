#include "lnArduino.h"
#include "lnSPI.h"
#include "gd32ST7735.h"
#include "FreeSans7pt7b.h"
#include "PNP_compressed.h"
#include "PNP_decl.h"

#define splash_width 128
#define splash_height 96


// green = PA1, blue = PA2, RED PC13
#define LED PA2

#define PINDC PB0
#define PINCS PB2
#define PINRST PB1

uint8_t ucHeap[configTOTAL_HEAP_SIZE];

extern "C"
{
    void _init();    
}

class Demo : public xTask
{
public:
        Demo(): xTask("Demo",5,250)
            {
                roundup=0;
            }
    void run (void);

    int roundup;    
};

void demoMe()
{
    hwlnSPIClass *spi=new hwlnSPIClass(0,-1);
    spi->begin();
    lnSPISettings transaction(8*1000*1000, SPI_MSBFIRST, SPI_MODE0,-1);
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
        vTaskDelay(500);
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
        Logger("*\n");
    }
    
}

void Demo::run()
{
    pinMode(LED,OUTPUT);
    bool onoff=true;
    digitalWrite(LED,HIGH);
    demoMe();
}

extern "C" void _exit(int x);
/**
 */
int main()
{
    // Initialize system
    _init();
    
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); //四位优先级组全配置为lvl
    eclic_global_interrupt_enable();                       //使能全局中断
    
    // The LEDs are all on GPIO A
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    // We need alternate functions too
    rcu_periph_clock_enable(RCU_AF); 
    rcu_periph_clock_enable(RCU_DMA0); 
    //
    DMA_CHCTL(DMA0, DMA_CH2) = (uint32_t)(DMA_PRIORITY_HIGH | DMA_CHXCTL_DIR); 
    //
    LoggerInit();
    Logger("Starting demo:\n");
    // Start freertos
    Demo *demo=new Demo;     
    vTaskStartScheduler();      
    deadEnd(25);
    
}

