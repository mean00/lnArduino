
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


#define CLOCK_XTAL_VALUE      8 // 8mhz quartz
#define CLOCK_TARGET_SYSCLOCK 108 // 108 Mhz
#define CLOCK_TARGET_PREDIV   2
/**
 * 
 * @param multiplier
 * @param predivider
 */
static void setPll(int multiplier, int predivider)
{
    volatile uint32_t *control=&(arcu->CTL);
    volatile uint32_t *cfg0=&(arcu->CFG0);
    volatile uint32_t *cfg1=&(arcu->CFG1);
    
    uint32_t c0=*cfg0;
    
    int pllMultiplier=multiplier;
    
    SystemCoreClock=(CLOCK_XTAL_VALUE*multiplier*1000000)/predivider;
    
    
    // Set PLL multiplier
    pllMultiplier--;
    c0&=~((0xf)<<18);
    c0&=~((0x1)<<29);
    c0|=(pllMultiplier&0x0f)<<18; // PLL Multiplier, ignore MSB
    c0|=(((pllMultiplier&0x10))>>4)<<29;
    *cfg0=c0;
    
         
    // Set PLL1 & PLL2
    uint32_t c1=0;            
    c1&=~LN_RCU_CFG1_PRED_CLOCKSEL; // 0-> XTAL is source for PREDV0
    c1|=LN_RCU_CFG1_PREDV0_DIV(1); // divide by 2
    c1|=LN_RCU_CFG1_PREDV1_DIV(1); // divide by 2
    c1|=LN_RCU_CFG1_PLL1_MUL(15);  // multipy by 20
    c1|=LN_RCU_CFG1_PLL2_MUL(15);  // multipy by 20
    *cfg1=c1;
                
    // enable pll1 & pll2, wait for stabilization
    *control |= LN_RCU_CTL_PLL1EN;
    waitControlBit(LN_RCU_CTL_PLL1STB);
    *control |= LN_RCU_CTL_PLL2EN;
    waitControlBit(LN_RCU_CTL_PLL2STB);    
}

//
void lnInitSystemClock()
{
    volatile uint32_t *control=&(arcu->CTL);
    volatile uint32_t *cfg0=&(arcu->CFG0);
    
    {
        *control=LN_RCU_CTL_IRC8MEN;     // Enable internal 8 Mhz oscillator
        *cfg0=0;         // select 8Mhz as source for sysclock
        *control=LN_RCU_CTL_IRC8MEN;     // Enable internal 8 Mhz oscillator
        arcu->INT=0;    // No interrupt
        arcu->CFG1=0;   // PLL pre divider not set
    }
    {
        // start crystal...
        *control|=LN_RCU_CTL_HXTALEN;
        waitControlBit(LN_RCU_CTL_HXTASTB); // Wait Xtal stable
    }
    {
        // Set HXTAL as source for PLL <<
        *cfg0=LN_RCU_CFG0_PLLSEL;
        setPll(27,2); // 8*27/2=108 Mhz
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
    
    // Enable PLL and wait for it to be stable
    *control|=LN_RCU_CTL_PLLEN;
    waitControlBit(LN_RCU_CTL_PLLSTB);
    
    // then switch to PLL as clock source
    uint32_t src=*cfg0;
    src&=~(LN_RCU_CFG0_SYSCLOCK_MASK); // clear source
    src|=LN_RCU_CFG0_SYSCLOCK_PLL; // set PLL as source
    *cfg0=src;
    
    waitCfg0Bit(LN_RCU_CFG0_PLL_USED);
    
}