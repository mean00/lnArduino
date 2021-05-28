/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "lnIRQ_priv.h"
/**
 * 
 */

#define ECLIC_CTLBIT 4      // We have the 4 higher bits
#define ECLIC_CTLBIT_SHIFT (8-ECLIC_CTLBIT) // # number of low bits
#define ECLIC_CTLBIT_LOW   ((1<<(ECLIC_CTLBIT_SHIFT))-1)
#define ECLIC_NB_SOURCES 87

void lnIrqSysInit()
{
    // start from a clean state
    *eclicCfg=0;
    *eclicMth=0;
        
    // Check the compilation value are the same as the runtime value
    xAssert((*eclicInfo &0xfff)==ECLIC_NB_SOURCES);
    xAssert(((*eclicInfo >>21)&0xf)==ECLIC_CTLBIT);
    
      // See bumblebee Core Architecture Manual
    *eclicCfg=4<<1; // nlbits =4
    
    for(int i=0;i<ECLIC_NB_SOURCES;i++)
    {
        eclicIrqs[i].ie=0;  // default : disabled
        eclicIrqs[i].control=ECLIC_CTLBIT_LOW;  // level 0 by default
        eclicIrqs[i].ip=0x0; 
        eclicIrqs[i].attr=0; // Default : Level trigger + Non Vector mode
    }
    
    
    // Switch to ECLIC mode
    // If the 6 lower bits= 3 => eclic mode
    uint32_t tmp1,tmp2;
    
    asm volatile (
                  "li  %0,0x3f\n" // https://stackoverflow.com/questions/64547741/how-is-risc-v-neg-instruction-imeplemented
                  "not %1,%0\n"
                  "csrr %0, mtvec\n" 
                  "and  %0,%0,%1\n" 
                  "ori  %0,%0,3\n"
                  "csrw mtvec, %0\n"     
            :: "r"(tmp1),"r"(tmp2)
    ); 
  
    
    // Preconfigure timer & syscall
    // these 2 are in vector mode
     eclicIrqs[7].attr=0xc1;
     eclicIrqs[3].attr=0xc1;
     eclicIrqs[7].control=0x1f; // level 1
     eclicIrqs[3].control=0x1f;    
    return;
}

void _enableDisable(bool enableDisable, const LnIRQ &irq)
{
    const _irqDesc *i=_irqs+(int)irq;
    xAssert(i->interrpt==irq)
    
    
    LN_ECLIC_irq *iclic=eclicIrqs+i->irqNb;
    if(enableDisable)
        iclic->ie|=1;
    else
        iclic->ie&=~1;
}
/**
 * 
 * @param irq
 */
extern "C" void lnEnableInterruptDirect(int irq)
{   
    LN_ECLIC_irq *iclic=eclicIrqs+irq;
    iclic->ie|=1;
}
/**
 * 
 * @param irq
 */
extern "C" void lnDisableInterruptDirect(int irq)
{   
    LN_ECLIC_irq *iclic=eclicIrqs+irq;
    iclic->ie&=~1;
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

void lnWriteMthDirect(int mth)
{
    *eclicMth=mth;
}
int  lnReadMthDirect()
{
    return *eclicMth;
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
    // No interrrupt
    ENTER_CRITICAL();
    while(1)
    {
        // blink red light...
        
    }
}        
// EOF

