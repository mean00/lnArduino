/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "lnIRQ_riscv_priv_ch32v3x.h"
/**
 * 
 */

struct CH32V3_INTERRUPT
{
    uint32_t ISR[4];                // 0x00 Interrupt Enable Status Register
    uint32_t dummy0[4];             // 0x10
    uint32_t IPR[4];                // 0x20 Interrupt Pending Status Register  
    uint32_t dummy1[4];             // 0x30
    uint32_t ITHRESHOLD;            // 0x40 Interrupt priority Threshold configuration register
    uint32_t dummy2;
    uint32_t CFGR      ;            // 0x48 Interrupt Configuration Register
    uint32_t GISR      ;            // 0x4C Global Status Register
    uint32_t VTFIDR    ;            // 0x50 VTF Interrupt ID configuration Register
    uint32_t dummy3[3]; 
    uint32_t VTFADDR[4]    ;        // 0x60 VTD Interrupt Address Register
    uint32_t dummy4[(0x100-0x70)/4]; 
    uint32_t IENR[4]    ;           // 0x100 Interrupt Enable Set Register
    uint32_t dummy5[(0x180-0x110)/4];
    uint32_t IRER[4]    ;           // 0x180 Interrupt Enable Reset Register
    uint32_t dummy6[(0x200-0x190)/4];
    uint32_t IPSR[4]    ;           // 0x200 Interrupt Pending Set Register
    uint32_t dummy7[(0x280-0x210)/4];
    uint32_t IPRR[4]    ;           // 0x280 Interrupt Pending Reset Register
    uint32_t dummy8[(0x300-0x290)/4];
    uint32_t IACTR[4]    ;          // 0x280 Interrupt Activation Register
    uint32_t dummy9[(0x400-0x310)/4];
    uint32_t IPRIOIR[64];          // 0x400 Priority(0..63)
};

CH32V3_INTERRUPT *pfic = (CH32V3_INTERRUPT *)LN_PFIC_ADR;

uint32_t underInterrupt=0;

// Attribute [LEVEL1:LEVEL0][SHV] : 
//      LEVEL: 
//              0:0=LEVEL, 
//              0:1=RISING EDGE, 
//              1:0: FALLINGEDGE   
//      SHV:
//              0 non vectored
//              1 vectored
//

void lnIrqSysInit()
{
   
    return;
}

void _enableDisable(bool enableDisable, const LnIRQ &irq)
{
    /*
    const _irqDesc *i=_irqs+(int)irq;
    xAssert(i->interrpt==irq)
    
    
    LN_ECLIC_irq *iclic=eclicIrqs+i->irqNb;
    if(enableDisable)
        iclic->ie|=1;
    else
        iclic->ie&=~1;
    LN_FENCE();
    */
}
/**
 * 
 * @param irq
 */
extern "C" void lnEnableInterruptDirect(int irq)
{   
   /* LN_ECLIC_irq *iclic=eclicIrqs+irq;
    iclic->ie|=1;
    LN_FENCE();
    */
}
/**
 * 
 * @param irq
 */
extern "C" void lnDisableInterruptDirect(int irq)
{   
    /*
    LN_ECLIC_irq *iclic=eclicIrqs+irq;
    iclic->ie&=~1;
    LN_FENCE();
    */
}

/**
 * 
 * @param per
 */
void lnEnableInterrupt(const LnIRQ &irq)
{   
    _enableDisable(true,irq);   
}
void lnIrqSetPriority(const LnIRQ &irq, int prio )
{    
  //  lnSetInterruptLevelDirect(irq,1+prio,false);
}
/**
 * 
 * @param irq
 */
void lnSetInterruptLevelDirect(int irq, int prio, bool vectored)
{
    /*
    LN_ECLIC_irq *iclic=eclicIrqs+irq;
    bool attr=0;
    if(vectored) attr|=1;
    // Assume level interrupt (?)
    iclic->attr=attr;
    // control
    //  4 bits then 1 1 1 1
    // nlbits=4 => level = 4 bits, priority=0 bits
    iclic->control=(prio<<ECLIC_CTLBIT_SHIFT)+ECLIC_CTLBIT_LOW;    
    LN_FENCE();
    */
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
   /* *eclicMth=mth;
    LN_FENCE();
    */
}
int  lnReadMthDirect()
{
    //return *eclicMth;
    return 0;
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


#ifdef LN_ENABLE_I2C
 void i2cIrqHandler(int instance, bool error);
#else
  #define i2cIrqHandler(...) deadEnd(1)
#endif

#define I2C_IRQ(d) extern "C"{ void I2C##d##_EV_IRQHandler(void) LN_INTERRUPT_TYPE ; void I2C##d##_EV_IRQHandler(void) { i2cIrqHandler(d,false);}} \
                 extern "C"{ void I2C##d##_ERR_IRQHandler(void)LN_INTERRUPT_TYPE;  void I2C##d##_ERR_IRQHandler(void) { i2cIrqHandler(d,true);} }

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

