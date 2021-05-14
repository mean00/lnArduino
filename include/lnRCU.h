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

