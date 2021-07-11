/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "lnIRQ_arm_priv.h"
#include "lnSCB_arm_priv.h"
#include "lnNVIC_arm_priv.h"
#include "lnIRQ_arm.h"
/**
 * 
 */

#define LN_NB_INTERRUPT 68
#define LN_VECTOR_OFFSET 16

LN_SCB_Registers *aSCB=(LN_SCB_Registers *)0xE000ED00;
static uint32_t interruptVector[LN_NB_INTERRUPT]  __attribute__((aligned(256)));

extern "C" void xPortPendSVHandler();
extern "C" void xPortSysTickHandler();
extern "C" void vPortSVCHandler();

static  volatile uint32_t curInterrupt;
static  volatile LnIRQ   curLnInterrupt;

volatile LN_NVIC *anvic=(LN_NVIC *)0xE000E100;
/**
 * \fn unsupportedInterrupt
 */
static void unsupportedInterrupt()
{
    curInterrupt=aSCB->ICSR &0xff;
    curLnInterrupt=(LnIRQ)(curInterrupt-LN_VECTOR_OFFSET);
    __asm__  ("bkpt 1");  
    xAssert(0);
}
#define LN_MSP_SIZE_UINT32  128
static uint32_t msp[LN_MSP_SIZE_UINT32]  __attribute__((aligned(8)));  // 128*4=512 bytes for msp

void lnIrqSetPriority(LnIRQ irq, int prio )
{
    int p=(prio&0xf)<<4;
    if(irq<LN_IRQ_WWDG) // Non IRQ
    {       
        // 2's complmenet
        uint32_t i=(uint32_t)irq;
        i&=0Xf;
        i-=4;
        aSCB->SHP[i]=p;
        return;
    }
    anvic->IP[irq]=p;   
}
extern "C" void USART0_IRQHandler ();
extern "C" void USART1_IRQHandler ();

/**
 * \fn lnIrqSysInit
 */
void lnIrqSysInit()
{
    //
    uint32_t unsupported=(uint32_t )unsupportedInterrupt;
    for(int i=0;i<LN_NB_INTERRUPT;i++)
        interruptVector[i]=unsupported;
    
    // by default disable everything
    anvic->ICER.data[0]=0xffffffffUL;
    anvic->ICER.data[1]=0xffffffffUL;
    anvic->ICER.data[2]=0xffffffffUL;
    anvic->ICER.data[3]=0xffffffffUL; 
    
    // Set priority to 14 for all interrupts
    for(int i=LN_IRQ_WWDG;i<LN_IRQ_ARM_LAST;i++)
        lnIrqSetPriority((LnIRQ)i,6);
    
    // Hook in SVC & friends
    interruptVector[0]=(uint32_t)&(msp[LN_MSP_SIZE_UINT32-1]);
    interruptVector[1]=(uint32_t)deadEnd;
    interruptVector[LN_VECTOR_OFFSET+LN_IRQ_PENDSV]=(uint32_t)xPortPendSVHandler;  // 16-2    14
    interruptVector[LN_VECTOR_OFFSET+LN_IRQ_SVCALL]=(uint32_t)vPortSVCHandler;     // 16-5    11
    interruptVector[LN_VECTOR_OFFSET+LN_IRQ_SYSTICK]=(uint32_t)xPortSysTickHandler;// 16-1    15
    
    interruptVector[LN_VECTOR_OFFSET+LN_IRQ_USART0]=(uint32_t)USART0_IRQHandler;// 16-1    15
    interruptVector[LN_VECTOR_OFFSET+LN_IRQ_USART1]=(uint32_t)USART1_IRQHandler;
    
    // Relocate vector to there    
    aSCB->VTOR = (uint32_t)interruptVector;
    __asm__ __volatile__("dsb sy") ;
    return;
}

void _enableDisable(bool enableDisable, const LnIRQ &zirq)
{
    int irq=(int)zirq;
    
    xAssert(irq>0) ;
    
    int offset=irq/32;
    int bit=irq&31;
    if(enableDisable)
    {
        anvic->ISER.data[offset]=1<<bit;
    }else
    {
        anvic->ICER.data[offset]=1<<bit;
    }
}
/**
 * 
 * @param per
 */
void lnEnableInterrupt(const LnIRQ &irq)
{   
    _enableDisable(true,irq);   
}
/**
 * 
 * @param per
 */
void lnDisableInterrupt(const LnIRQ &irq)
{
     _enableDisable(false,irq);
}


#define DMA_IRQ(d,c) extern "C" void DMA##d##_Channel##c##_IRQHandler(void) { dmaIrqHandler(d,c);}
/**
 * 
 * @param dma
 * @param channel
 */
void dmaIrqHandler(int dma, int channel);
DMA_IRQ(0,0)
DMA_IRQ(0,1)
DMA_IRQ(0,2)
DMA_IRQ(0,3)
DMA_IRQ(0,4)
DMA_IRQ(0,5)
DMA_IRQ(0,6)
DMA_IRQ(1,0)
DMA_IRQ(1,1)
DMA_IRQ(1,2)
DMA_IRQ(1,3)
DMA_IRQ(1,4)
DMA_IRQ(1,5)
DMA_IRQ(1,6)
  
/**
 * I2C
 * @param code
 */     
void i2cIrqHandler(int instance, bool error);
#define I2C_IRQ(d) extern "C" void I2C##d##_EV_IRQHandler(void) { i2cIrqHandler(d,false);} \
                   extern "C" void I2C##d##_ERR_IRQHandler(void) { i2cIrqHandler(d,true);} 

I2C_IRQ(0)
I2C_IRQ(1)

extern "C" void deadEnd(int code)
{
    __asm__  ("bkpt 1");  
    // No interrrupt
    ENTER_CRITICAL();
    while(1)
    {
        // blink red light...
        
    }
}        
// EOF
