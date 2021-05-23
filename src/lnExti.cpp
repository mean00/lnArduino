/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnGPIO.h"
#include "lnExti.h"
#include "lnExti_priv.h"
#include "lnPeripheral_priv.h"

LN_EXTI_Register *aExiti=(LN_EXTI_Register *)LN_EXTI_ADR ;

#define LN_EXTI_NB_SOURCES 19

/**
 */
struct _lnExtidescriptor
{
    int port;
    lnExtiCallback *cb;
    void           *cookie;
};
static _lnExtidescriptor _extiDesc[LN_EXTI_NB_SOURCES];
/**
 * 
 */
void lnExtiSysInit()
{
    for(int i=0;i<LN_EXTI_NB_SOURCES;i++)
    {
        _lnExtidescriptor *d=_extiDesc+i;
        d->cb=NULL;
        d->cookie=NULL;
        d->port=-1;
    }
    aExiti->RTEN=0;
    aExiti->FTEN=0;
    aExiti->EVEN=0;
    aExiti->INTEN=0;
    lnEnableInterrupt(LN_IRQ_EXTI0);
    lnEnableInterrupt(LN_IRQ_EXTI1);
    lnEnableInterrupt(LN_IRQ_EXTI2);
    lnEnableInterrupt(LN_IRQ_EXTI3);
    lnEnableInterrupt(LN_IRQ_EXTI4);
    lnEnableInterrupt(LN_IRQ_EXTI5_9);
    lnEnableInterrupt(LN_IRQ_EXTI10_15);
}

/**
 * 
 * @param pin
 * @param edge
 * @param cb
 * @param cookie
 */
void lnExtiAttachInterrupt(const lnPin pin, const lnEdge edge, lnExtiCallback cb, void *cookie)
{
    // Check if already used ?
    int port=pin>>4;
    int source=pin&0xf;
    _lnExtidescriptor *d=_extiDesc+source;
    if(d->port!=-1)
    {
        xAssert(d->port==port);
    }
    d->port=port;
    d->cb=cb;
    d->cookie=cookie;
    
    // Program edge
    if(edge & 1) //Rising
    {
        aExiti->RTEN|=1<<source;
    }else
    {
        aExiti->RTEN&=~(1<<source);
    }
    if(edge & 2) //Falling
    {
        aExiti->FTEN|=1<<source;
    }else
    {
        aExiti->FTEN&=~(1<<source);
    }
    
}
/**
 * 
 * @param pin
 */
void lnExtiDetachInterrupt(const lnPin pin)
{
    int port=pin>>4;
    int source=pin&0xf;
    _lnExtidescriptor *d=_extiDesc+source;
    xAssert(port==d->port);
    d->cb=NULL;
    d->port=-1;
    aExiti->INTEN&=~(1<<source);
    aExiti->RTEN&=~(1<<source);
    aExiti->FTEN&=~(1<<source);
            
}
/**
 * 
 * @param pin
 */
void lnExtiEnableInterrupt(const lnPin pin)
{
    int port=pin>>4;
    int source=pin&0xf; 
    _lnExtidescriptor *d=_extiDesc+source;
    xAssert(port==d->port);
    aExiti->INTEN|=(1<<source);
}
/**
 * 
 * @param pin
 */
void lnExtiDisableInterrupt(const lnPin pin)
{
    int port=pin>>4;
    int source=pin&0xf; 
    _lnExtidescriptor *d=_extiDesc+source;
    xAssert(port==d->port);
    aExiti->INTEN&=~(1<<source);
}

void EXTI_IrqHandler(int maskS,int maskE)
{
    /***/
    for(int i=maskS;i<maskE;i++)
    {
         _lnExtidescriptor *d=_extiDesc+i;
         // is it pending & enabled ?
         int pe=aExiti->INTEN & aExiti->PD & (1<<i);
         if(!pe) continue;
         // yes, clear it
         aExiti->PD=1<<i;
         // call handler
         xAssert(d->cb);
         d->cb(d->port*16+i,d->cookie);         
    }
}

extern "C" 
{
    void EXTI0_IRQHandler()     {EXTI_IrqHandler(0,0);}
    void EXTI1_IRQHandler()     {EXTI_IrqHandler(1,1);}
    void EXTI2_IRQHandler()     {EXTI_IrqHandler(2,2);}
    void EXTI3_IRQHandler()     {EXTI_IrqHandler(3,3);}
    void EXTI4_IRQHandler()     {EXTI_IrqHandler(4,4);}
    void EXTI5_9_IRQHandler()   {EXTI_IrqHandler(5,9);}
    void EXTI10_15_IRQHandler() {EXTI_IrqHandler(10,15);}
}

// EOF

