/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"


extern LN_RCU *arcu;
// 

uint32_t _rcuClockApb1=108000000/2;
uint32_t _rcuClockApb2=108000000;
extern "C" uint32_t SystemCoreClock;
uint32_t SystemCoreClock=0;
/**
 * 
 * @param periph
 * @return 
 */
uint32_t lnPeripherals::getClock(const Peripherals periph)
{
    switch(periph)
    {
        case pTIMER5:
        case pTIMER6:
                    return _rcuClockApb1*2;
        case pUART0:
        case pTIMER0:
        case pSPI0:
        case pAF:
                    return _rcuClockApb2;
        default:
                    return _rcuClockApb1;
    }
}
/**
 * \brief wait for the "stabilized" bit to be set
 * @param bit
 */

static void waitControlBit(int mask)
{
    while(!(arcu->CTL & mask))
    {
        __asm__("nop");
    }
    
}

static void waitCfg0Bit(int mask)
{
    
    while(!(arcu->CFG0 & mask))
    {
        __asm__("nop");
    }
    
}


#define CLOCK_XTAL_VALUE        8 // 8mhz quartz
#define CLOCK_TARGET_SYSCLOCK   (LN_MCU_SPEED/(1000*1000))  // 108 Mhz
#define CLOCK_TARGET_PREDIV     1

//{CTL = 0x38683, CFG0 = 0x400, CIR = 0x0, APB2RSTR = 0x0, APB1RSTR = 0x0, AHBENR = 0x14, APB2ENR = 0x0, APB1ENR = 0x0, BDCR = 0x18, CSR = 0x1c000000}
//{CTL = 0x38683, CFG0 = 0x1d0400, CIR = 0x0, APB2RSTR = 0x0, APB1RSTR = 0x0, AHBENR = 0x14, APB2ENR = 0x0, APB1ENR = 0x0, BDCR = 0x18, CSR = 0x1c000000}
//{CTL = 0x3038683, CFG0 = 0x1d0400, CIR = 0x0, APB2RSTR = 0x0, APB1RSTR = 0x0, AHBENR = 0x14, APB2ENR = 0x0, APB1ENR = 0x0, BDCR = 0x18, CSR = 0x1c000000}

/**
 * 
 * @param multiplier
 * @param predivider
 */

static const int Multipliers[10]={
    0,0,0,0, // 0 1 2 3
    2,3,4,5,6,7 // 4 5 6 7 8 9
};

static void setPll(int multiplier, int predivider)
{
    volatile uint32_t *control=&(arcu->CTL);
    volatile uint32_t *cfg0=&(arcu->CFG0);
    volatile uint32_t *cfg1=&(arcu->CFG1);
    
    uint32_t c0=*cfg0;
    
    int pllMultiplier=Multipliers[multiplier];
    
    SystemCoreClock=(CLOCK_XTAL_VALUE*multiplier*1000000)/predivider;
    _rcuClockApb1=SystemCoreClock/2;
    _rcuClockApb2=SystemCoreClock;
    
    // Set PLL multiplier    
    c0&=~((0xf)<<18);    
    c0|=((pllMultiplier)&0x0f)<<18; // PLL Multiplier, ignore MSB        
    c0 |=LN_RCU_CFG0_PLLSEL;
    *cfg0=c0;
}

//
void lnInitSystemClock()
{
    volatile uint32_t *control=&(arcu->CTL);
    volatile uint32_t *cfg0=&(arcu->CFG0);
    
       
    {
        // start crystal...
        *control|=LN_RCU_CTL_HXTALEN;
        waitControlBit(LN_RCU_CTL_HXTASTB); // Wait Xtal stable
    }
     {
        setPll(CLOCK_TARGET_SYSCLOCK/CLOCK_XTAL_VALUE,CLOCK_TARGET_PREDIV); // 8*9/1=72 Mhz        
    }
    {
        // start PLL...
        *control|=LN_RCU_CTL_PLLEN;
        waitControlBit(LN_RCU_CTL_PLLSTB); // Wait Xtal stable
    }
    
   
    // Setup AHB...
    // AHB is Xtal:1, divider value=0    
    // APB2=AHB/1
    uint32_t a=*cfg0;
    a&=~((0xf)<<4); // AHB PRESCALER CLEAR
    a|=0<<4;      // AHB prescaler is CK_SYS
    a&=~(3<<11); //  APB2 Prescaler clear
    a|=0<<11;     // APB2 is AHB
    *cfg0=a;
    // APB1=AHB/2, divider value=4
    a&=~(7<<8);
    a|=4<<8;        // APB1 is AHB/2
    *cfg0=a;          
    
    // now switch system clock to pll
    a&=~(LN_RCU_CFG0_SYSCLOCK_MASK);
    a|=LN_RCU_CFG0_SYSCLOCK_PLL;
    *cfg0=a;       
}
// EOF