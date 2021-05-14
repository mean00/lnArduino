/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */


#pragma once

 enum Peripherals
    {
            pNONE=0,
            pSPI0,
            pSPI1,
            pSPI2,
            pUART0,
            pUART1,
            pUART2,
            pUART3,
            pUART4,    
            pI2C0,    
            pI2C1,    
            pCAN0,    
            pCAN1,    
            pDAC,           
            pPMU,        
            pBKPI,
            pWWDGT,
            pTIMER0,
            pTIMER1,
            pTIMER2,
            pTIMER3,
            pTIMER4,
            pTIMER5,
            pTIMER6,
            pADC0,
            pADC1,
            pGPIOA,
            pGPIOB,
            pGPIOC,
            pGPIOD,
            pGPIOE,
            pAF,
            pDMA0,
            pDMA1
    };

class lnPeripherals
{
public:
   
public:            
    static void reset(const Peripherals periph);
    static void enable(const Peripherals periph);
    static void disable(const Peripherals periph);
    static uint32_t getClock(const Peripherals periph);
};

void lnInitSystemClock();

/* Periphderal ADDRESS */
#warning FIXME
#define LN_DMA0_ADR     (0x40020000)
#define LN_DMA1_ADR     (0x40020400)
#define LN_USART0_ADR   (0x40013800)
#define LN_USART1_ADR   (0x40004400)
#define LN_USART2_ADR   (0x40004800)
#define LN_USART3_ADR   (0x40004C00)
#define LN_USART4_ADR   (0x40005000)
#define LN_SPI0_ADR     (0x40013000)
#define LN_SPI1_ADR     (0x40003800)
#define LN_SPI2_ADR     (0x40003C00)
#define LN_RCU_ADR      (0x40021000)
#define LN_GPIOA_ADR    (0x40010800)
#define LN_GPIOB_ADR    (0x40010C00)
#define LN_GPIOC_ADR    (0x40011000)
#define LN_GPIOD_ADR    (0x40011400)
#define LN_GPIOE_ADR    (0x40011800)
#define LN_AFIO_ADR     (0x40010000)

