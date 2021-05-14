
/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once

struct LN_RCU
{
    uint32_t CTL;
    uint32_t CFG0;
    uint32_t INT;
    uint32_t APB2RST;
    uint32_t APB1RST;
    uint32_t AHBEN;
    uint32_t APB2EN;
    uint32_t APB1EN;
    uint32_t BDCTL;
    uint32_t RSTCLK;
    uint32_t AHBRST;
    uint32_t CFG1;
    uint32_t DSV;
};


// CTL
#define LN_RCU_CTL_IRC8MEN    (1<<0)
#define LN_RCU_CTL_IRC8MSTB   (1<<1)
#define LN_RCU_CTL_HXTALEN    (1<<16)
#define LN_RCU_CTL_HXTASTB    (1<<17)
#define LN_RCU_CTL_HXTABPS    (1<<18)
#define LN_RCU_CTL_CKMEN      (1<<19)
#define LN_RCU_CTL_PLLEN      (1<<24)
#define LN_RCU_CTL_PLLSTB     (1<<25)
#define LN_RCU_CTL_PLL1EN     (1<<26)
#define LN_RCU_CTL_PLL1STB    (1<<27)
#define LN_RCU_CTL_PLL2EN     (1<<28)
#define LN_RCU_CTL_PLL2STB    (1<<29)

// APB1 EN // APB1 RESET
#define LN_RCU_APB1_TIMER1EN  (1<<0)
#define LN_RCU_APB1_TIMER2EN  (1<<1)
#define LN_RCU_APB1_TIMER3EN  (1<<2)
#define LN_RCU_APB1_TIMER4EN  (1<<3)
#define LN_RCU_APB1_TIMER5EN  (1<<4)
#define LN_RCU_APB1_TIMER6EN  (1<<5)
#define LN_RCU_APB1_WWDGTEN   (1<<11)
#define LN_RCU_APB1_SPI1EN    (1<<14)
#define LN_RCU_APB1_SPI2EN    (1<<15)
#define LN_RCU_APB1_USART1EN  (1<<17)
#define LN_RCU_APB1_USART2EN  (1<<18)
#define LN_RCU_APB1_USART3EN  (1<<19)
#define LN_RCU_APB1_USART4EN  (1<<20)
#define LN_RCU_APB1_I2C0EN    (1<<21)
#define LN_RCU_APB1_I2C1EN    (1<<22)
#define LN_RCU_APB1_CAN0EN    (1<<25)
#define LN_RCU_APB1_CAN1EN    (1<<26)
#define LN_RCU_APB1_BKPIEN    (1<<27)
#define LN_RCU_APB1_PMUEN     (1<<28)
#define LN_RCU_APB1_DACEN     (1<<29)

// APB2 EN // APB2 RST

#define LN_RCU_APB2_AFEN      (1<<0)
#define LN_RCU_APB2_PAEN      (1<<2)
#define LN_RCU_APB2_PBEN      (1<<3)
#define LN_RCU_APB2_PCEN      (1<<4)
#define LN_RCU_APB2_PDEN      (1<<5)
#define LN_RCU_APB2_PEEN      (1<<6)

#define LN_RCU_APB2_ADC0EN    (1<<9)
#define LN_RCU_APB2_ADC1EN    (1<<10)
#define LN_RCU_APB2_TIMER0EN  (1<<11)
#define LN_RCU_APB2_SPI0EN    (1<<12)
#define LN_RCU_APB2_USART0EN  (1<<14)

// AHBEN

#define LN_RCU_AHB_DMA0EN      (1<<0)
#define LN_RCU_AHB_DMA1EN      (1<<1)
#define LN_RCU_AHB_SRAMSP      (1<<2)
#define LN_RCU_AHB_FMCSPEN     (1<<4)
#define LN_RCU_AHB_CRCEN       (1<<6)
#define LN_RCU_AHB_EXMCEN      (1<<8)
#define LN_RCU_AHB_USBFSEN     (1<<12)

// AHB RESET
#define LN_RCU_AHBRST_USBFSRST      (1<<12)

// CFG0
#define LN_RCU_CFG0_PLL_USED   (1<<3)
#define LN_RCU_CFG0_SYSCLOCK_MASK (3)
#define LN_RCU_CFG0_SYSCLOCK_PLL (1<<1)


// CFG1
#define LN_RCU_CFG1_PRED_CLOCKSEL   (1<<16)
#define LN_RCU_CFG1_PLL2_MUL(x)   ((x)<<12)
#define LN_RCU_CFG1_PLL1_MUL(x)   ((x)<<8)
#define LN_RCU_CFG1_PREDV1_DIV(x)   ((x)<<4)
#define LN_RCU_CFG1_PREDV0_DIV(x)   ((x)<<0)

