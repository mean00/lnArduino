/**
 * @file lnRCU_systemClock_riscv_ch32v3x.cpp
 * @author MEAN00 fixounet@free.fr
 * @brief  This deals with the clock tree (except usb)
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021-2024
 * 
 */
#include "lnArduino.h"
#include "lnCpuID.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"

extern LN_RCU *arcu;
//

#define CLOCK_XTAL_VALUE 8                                   // 8mhz quartz
#define CLOCK_TARGET_SYSCLOCK (LN_MCU_SPEED / (1000 * 1000)) // 108 Mhz
#define CLOCK_TARGET_PREDIV 1
//
#define LN_CLOCK_IRC8 0
#define LN_CLOCK_XTAL 16
#define LN_CLOCK_PLL 24
//
#define CH32_CFG0_PLL_MASK (0xf<<18)
#define CH32_CFG0_PLL_VALUE(x) ((x)<<18)
#define CH32_CFG0_PLL_EXTERNAL (1<<16)

//
uint32_t _rcuClockApb1 = 108000000 / 2;
uint32_t _rcuClockApb2 = 108000000;
extern "C" uint32_t SystemCoreClock;
uint32_t SystemCoreClock = 0;
/**
 *
 * @param periph
 * @return
 */
uint32_t lnPeripherals::getClock(const Peripherals periph)
{
    switch (periph)
    {
    case pTIMER5:
    case pTIMER6:
        return _rcuClockApb1 * 2;
        break;
    case pUART0:
    case pTIMER0:
    case pSPI0:
    case pAF:
        return _rcuClockApb2;
        break;
    case pAPB1:
        return _rcuClockApb1;
        break;
    case pAPB2:
        return _rcuClockApb2;
        break;
    case pSYSCLOCK:
        return SystemCoreClock;
        break;
    default:
        return _rcuClockApb1;
        break;
    }
}

/**
 * \brief wait for the "stabilized" bit to be set
 * @param bit
 */

static void waitControlBit(int mask)
{
    while (!(arcu->CTL & mask))
    {
        __asm__("nop");
    }
}
/**
 * @brief 
 * 
 * @param mask 
 */
static void waitCfg0Bit(int mask)
{

    while (!(arcu->CFG0 & mask))
    {
        __asm__("nop");
    }
}

/**
 *
 * @param multiplier
 * @param predivider
 * \briefs this translates the wanted value into value to put in
 *  i.e. if you want x10, you use multipliers[10]
 */

static const uint8_t Multipliers[] = {
    0, 0, 0, 1, // 0 1 2 3
    2, 3, 4, 5, // 4 5 6 7
    6, 7, 8, 9, // 8 9 10 11
    // the ones below dont work on stm32 !
    10, 11, 12, 13, // 12 13 14 15
    14, 14, 15      // 16 17 18 19
};
/**
 *
 * @param multiplier
 * @param predivider
 * @param external
 */
void setPll(int inputClock, int multiplier, int predivider, bool external)
{    
    arcu->CFG1 = 0;
    xAssert(multiplier < sizeof(Multipliers));
    int pllMultiplier = Multipliers[multiplier];

    SystemCoreClock = (inputClock * multiplier * 1000000) / predivider;
    _rcuClockApb1 = SystemCoreClock / 2;
    _rcuClockApb2 = SystemCoreClock;

    // Set PLL multiplier
    uint32_t c0 = arcu->CFG0;
    c0 &= CH32_CFG0_PLL_MASK;
    c0 |= CH32_CFG0_PLL_VALUE(pllMultiplier); // PLL Multiplier, ignore MSB

    if (external)
        c0 |= CH32_CFG0_PLL_EXTERNAL;
    else
        c0 &= ~CH32_CFG0_PLL_EXTERNAL;
    arcu->CFG0 = c0;

    // start PLL...
    arcu->CTL |= LN_RCU_CTL_PLLEN;
    waitControlBit(LN_RCU_CTL_PLLSTB); // Wait Xtal stable
}

/**
 * @brief 
 * 
 * @param clock 
 * @param enabled 
 */
static void enableDisableClock(int clock, bool enabled)
{
    int setBit = 1 << clock;
    int statusBit = 1 << (clock + 1);
    if (enabled)
    {
        arcu->CTL |= setBit;
        waitControlBit(statusBit);
    }
    else
    {
        arcu->CTL &= ~setBit;
    }
}
/**
 * @brief 
 * 
 */
void lnInitSystemClock()
{
    lnCpuID::identify();

    //________________________
    // Reset everything...
    //________________________
    // Enable IRC8 just in case
    enableDisableClock(LN_CLOCK_IRC8, true);
    // Switch SYS clock to IRC8
    uint32_t sysClock = arcu->CFG0;
    sysClock &= ~LN_RCU_CFG0_SYSCLOCK_MASK; //  0 is IRC8
    arcu->CFG0 = sysClock;
    while (1)
    {
        if (!(arcu->CFG0 & (LN_RCU_CFG0_SYSCLOCK_MASK << 2)))
            break;
    }
    // Ok now we can disable PLL and Xtal
    enableDisableClock(LN_CLOCK_XTAL, false);
    enableDisableClock(LN_CLOCK_PLL, false);

    //________________________
    //
    //________________________

    volatile uint32_t *control = &(arcu->CTL);
    volatile uint32_t *cfg0 = &(arcu->CFG0);

    int inputClock;
    bool useExternalClock;

#ifndef LN_USE_INTERNAL_CLOCK
    {
        // start crystal...
        *control |= LN_RCU_CTL_HXTALEN;
        waitControlBit(LN_RCU_CTL_HXTASTB); // Wait Xtal stable
        inputClock = CLOCK_XTAL_VALUE;
        useExternalClock = true;
    }
#else
    inputClock = 8 / 2; // HSI is divided by 2
    useExternalClock = false;
#endif

    int multiplier = CLOCK_TARGET_SYSCLOCK / inputClock;
    setPll(inputClock, multiplier, CLOCK_TARGET_PREDIV, useExternalClock); // 8*9/1=72 Mhz

    // Setup AHB...
    // AHB is Xtal:1, divider value=0
    // APB2=AHB/1
    uint32_t a = *cfg0;
    a &= ~((0xf) << 4); // AHB PRESCALER CLEAR
    a |= 0 << 4;        // AHB prescaler is CK_SYS
    a &= ~(3 << 11);    //  APB2 Prescaler clear
    a |= 0 << 11;       // APB2 is AHB
    *cfg0 = a;
    // APB1=AHB/2, divider value=4
    a &= ~(7 << 8);
    a |= 4 << 8; // APB1 is AHB/2
    *cfg0 = a;

    // it it is a STM32 chip, increase flash wait state
    // GD has ram, so  0 wait state is fine
    switch (lnCpuID::vendor())
    {
    case lnCpuID::LN_MCU_STM32: {
        int ws = 0;
        if (CLOCK_TARGET_SYSCLOCK > 48)
            ws = 2;
        else if (CLOCK_TARGET_SYSCLOCK > 24)
            ws = 1;

        *(uint32_t *)0x40022000 = 0x30 + ws; // enable prefetch
    }
    break;
    default:
    case lnCpuID::LN_MCU_CH32:
    case lnCpuID::LN_MCU_GD32:
        break;
    }

    // now switch system clock to pll
    a &= ~(LN_RCU_CFG0_SYSCLOCK_MASK);
    a |= LN_RCU_CFG0_SYSCLOCK_PLL;
    *cfg0 = a;

    // In debug mode, halt everthing we can
    volatile uint32_t *debug = (volatile uint32_t *)0xE0042000U;
    // stop timer 0..3 i2c0 i2c1  timer 4..7 timer 8..13
    // jtagId=debug[0];
    // debug[1]=(0xf<<10)+(1<<15)+(0x1f<<16)+(0x3f<<25);
} // EOF
