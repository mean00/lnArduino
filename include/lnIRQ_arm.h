/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"
#include "lnPeripherals.h"

#define LN_IRQ_OFFSET 0

enum LnIRQ
{
    // These are exception, not really IRQS
    // but it"s simpler to manage the vector table
    LN_IRQ_MSIP            =LN_IRQ_OFFSET-13,
    LN_IRQ_NONE            =LN_IRQ_MSIP,
    LN_IRQ_RESET           =LN_IRQ_OFFSET-12,
    LN_IRQ_NMI             =LN_IRQ_OFFSET-11,
    LN_IRQ_HARDFAULT       =LN_IRQ_OFFSET-10,
    LN_IRQ_MEMMANAGE       =LN_IRQ_OFFSET-9,
    LN_IRQ_BUSFAULT        =LN_IRQ_OFFSET-8,
    LN_IRQ_USAGEFAULT      =LN_IRQ_OFFSET-7,
    LN_IRQ_RESERVED        =LN_IRQ_OFFSET-6,
    LN_IRQ_SVCALL          =LN_IRQ_OFFSET-5,
    LN_IRQ_DEBUGMON        =LN_IRQ_OFFSET-4,
    LN_IRQ_RESERVED2       =LN_IRQ_OFFSET-3,
    LN_IRQ_PENDSV          =LN_IRQ_OFFSET-2,
    LN_IRQ_SYSTICK         =LN_IRQ_OFFSET-1,
    
    // The real IRQs start here
    
    LN_IRQ_WWDG            =LN_IRQ_OFFSET, // 0+13=13
    LN_IRQ_LVD            ,
    LN_IRQ_TAMPER         ,
    LN_IRQ_RTC            ,
    LN_IRQ_FMC           ,
    LN_IRQ_RCU_RCC        ,
    LN_IRQ_EXTI0          ,
    LN_IRQ_EXTI1          ,
    LN_IRQ_EXTI2          ,
    LN_IRQ_EXTI3          ,
    LN_IRQ_EXTI4          ,
    LN_IRQ_DMA0_Channel0  ,
    LN_IRQ_DMA0_Channel1        ,
    LN_IRQ_DMA0_Channel2        ,
    LN_IRQ_DMA0_Channel3        ,
    LN_IRQ_DMA0_Channel4        ,
    LN_IRQ_DMA0_Channel5 =29    , // 16+13=29
    LN_IRQ_DMA0_Channel6        ,
    LN_IRQ_ADC0_1        ,
    LN_IRQ_USB_HP_CAN_TX  ,
    LN_IRQ_USB_LP_CAN_RX0 ,
    LN_IRQ_CAN_RX1        ,
    LN_IRQ_CAN_SCE        ,
    LN_IRQ_EXTI5_9       ,
    LN_IRQ_TIMER0_BRK ,
    LN_IRQ_TIMER0_UP ,
    LN_IRQ_TIMER0_TRG_CMT ,
    LN_IRQ_TIMER0_Channel      , //40
    LN_IRQ_TIMER1         ,
    LN_IRQ_TIMER2         ,
    LN_IRQ_TIMER3         ,
    LN_IRQ_I2C0_EV        ,
    LN_IRQ_I2C0_ER        ,
    LN_IRQ_I2C1_EV        ,
    LN_IRQ_I2C1_ER        ,
    LN_IRQ_SPI0           ,
    LN_IRQ_SPI1           ,
    LN_IRQ_USART0         , //50
    LN_IRQ_USART1         ,
    LN_IRQ_USART2     =52    , // 39+13=52
    LN_IRQ_EXTI10_15     ,
    LN_IRQ_RTCALARM       ,
    LN_IRQ_USBWAKEUP      ,
    LN_IRQ_TIMER8_BRK_TIMER12 ,
    LN_IRQ_TIMER8_UP_TIMER13 , //57
    LN_IRQ_TIMER8_TRG_COM_TIMER14 ,
    LN_IRQ_TIMER8_CC      ,
    LN_IRQ_ADC3           , // 47+13=60
    LN_IRQ_FSMC           ,
    LN_IRQ_SDIO           ,
    LN_IRQ_TIMER5         ,
    LN_IRQ_SPI2           ,
    LN_IRQ_UART3          ,
    LN_IRQ_UART4          ,
    LN_IRQ_TIMER6         ,
    LN_IRQ_TIMER7         ,   // 55+13=68
    LN_IRQ_DMA1_Channel0       ,   // 56
    LN_IRQ_DMA1_Channel1       ,   // 57
    LN_IRQ_DMA1_Channel2       ,   // 58 
    LN_IRQ_DMA1_Channel3    ,  //  59 +13 =72
    LN_IRQ_DMA1_Channel4    ,

};


void lnSetInterruptLevelDirect(int intr, int prio, bool vectored);
