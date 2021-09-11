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
extern "C" void __exc_reset();

static  volatile uint32_t curInterrupt;
static  volatile LnIRQ   curLnInterrupt;

volatile LN_NVIC *anvic=(LN_NVIC *)0xE000E100;

#define LN_MSP_SIZE_UINT32  128
static uint32_t msp[LN_MSP_SIZE_UINT32]  __attribute__((aligned(8)));  // 128*4=512 bytes for msp


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
/**
 * 
 * @param irq
 * @param prio
 */
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


/**
 * 
 * @param enableDisable
 * @param zirq
 */
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
//
// Forward declaration of interrupt handlers
//

extern "C" void USART0_IRQHandler ();
extern "C" void USART1_IRQHandler ();
extern "C" void USART2_IRQHandler ();


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

extern "C"
{
  extern   void EXTI0_IRQHandler()   ;
  extern   void EXTI1_IRQHandler() ;
  extern   void EXTI2_IRQHandler() ;
  extern   void EXTI3_IRQHandler() ;
  extern   void EXTI4_IRQHandler() ;
  extern   void EXTI5_9_IRQHandler();
  extern   void EXTI10_15_IRQHandler();
}
/**
 * 
 * @param code
 */
extern "C" void deadEnd(int code)
{
    static int lastErrorCode;
    __asm__("cpsid if");  // disable interrupt
    lastErrorCode=code;
    __asm__  ("bkpt 1");      
    while(1)
    {
        // blink red light...
        __asm__("nop");
        __asm__("nop"); // do something to avoid deadlock
    }
}    
// https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-processor/exception-model/exception-entry-and-return
// https://interrupt.memfault.com/blog/arm-cortex-m-exceptions-and-nvic

struct registerStack
{
    uint32_t R0,R1,R2,R3,R12,LR,PC,xPSR;
};
volatile registerStack *crashStructure;
/**
 * 
 * @param sp
 */
extern "C" void crashHandler2(void *sp)  __attribute__((used)) __attribute__((naked ));
/**
 * 
 * @param sp
 */
void crashHandler2(void *sp)  
{
    crashStructure=(registerStack *)sp;
    __asm__("cpsid if    \n" );
    deadEnd(0x90);
}

/**
 * 
 * @param code
 */
void crashHandler(int code)  __attribute__((used)) __attribute__((naked ));

// https://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html   
#define IRQ_STUBS(name,code) extern "C" void name() __attribute__((used)) __attribute__((naked )); extern "C" void name()   \
{ \
    __asm__ \
    ( \
        " tst lr, #4                                                \n" \
        " ite eq                                                    \n" \
        " mrseq r0, msp                                             \n" \
        " mrsne r0, psp                                             \n" \
        " ldr r2, handler2_address_const"#name"                     \n" \
        " bx r2                                                     \n" \
        " handler2_address_const"#name": .word crashHandler2    \n" ::  \
    ); \
}
/**
 * 
 * @param code
 */

IRQ_STUBS(RTC_IrqHandler,0x21);
IRQ_STUBS(WDG_IrqHandler,0x22);
IRQ_STUBS(Tamper_IrqHandler,0x23);
IRQ_STUBS(RCU_IrqHandler,0x24);
IRQ_STUBS(FMC_IrqHandler,0x25);
IRQ_STUBS(HardFault_IrqHandler,0x26);
IRQ_STUBS(NMI_IrqHandler,0x27);
IRQ_STUBS(MemManage_IrqHandler,0x28);
IRQ_STUBS(BusFault_IrqHandler,0x29);
IRQ_STUBS(UsageFault_IrqHandler,0x2a);
IRQ_STUBS(DebugMon_IrqHandler,0x2b);

#define unsupported unsupportedInterrupt
#include "lnIRQ_arm_vector.h"

/**
 * \fn lnIrqSysInit
 */
void lnIrqSysInit()
{
    // by default disable everything
    anvic->ICER.data[0]=0xffffffffUL;
    anvic->ICER.data[1]=0xffffffffUL;
    anvic->ICER.data[2]=0xffffffffUL;
    anvic->ICER.data[3]=0xffffffffUL; 
    
    // Set priority to 14 for all interrupts
    for(int i=LN_IRQ_WWDG;i<LN_IRQ_ARM_LAST;i++)
        lnIrqSetPriority((LnIRQ)i,6);
    
    // Relocate vector to there    
    aSCB->VTOR = (uint32_t)LnVectorTable;
    __asm__ __volatile__("dsb sy") ;
    return;
}
// EOF
