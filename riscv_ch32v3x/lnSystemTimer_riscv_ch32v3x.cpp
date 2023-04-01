
#include "lnArduino.h"
#include "lnSystemTimer_priv.h"
#include "lnPeripheral_priv.h"



typedef struct
{
    uint32_t CTLR;
    uint32_t SR;
    uint32_t CTNTL;
    uint32_t CTNTH;
}lnStkx;

typedef  volatile lnStkx lnStk;

lnStk *stk = (lnStk *)LN_STK_ADR;
static uint64_t tickPerUs16=1;

/**
 *  This should be called with interrupt disabled
 */
void lnSystemTimerInit()
{
    // number of ticks for a duration of 1 us
    tickPerUs16=((SystemCoreClock*4))/(1000*1000); // *16/4    
}
/**
 */
uint32_t lnGetCycle32()
{
    return stk->CTNTL;
}
/**
 */
uint64_t lnGetCycle64()
{
    volatile uint32_t high,low;
    while(1)
    {
        high= stk->CTNTH;
        low=  stk->CTNTL;
        volatile  uint32_t high2=stk->CTNTH;
        if(high==high2)
        {
            break;
        }
    }
    uint64_t r=high;
    r<<=32;
    r+=low;
    return r;
}


/**
 * 
 * @return 
 */
uint64_t lnGetUs64()
{    
    uint64_t tick=lnGetCycle64();
    // convert tick to us
    tick=(tick*16)/tickPerUs16;
    tick=tick+1000*lnGetMs();
    return tick;    
}
/**
 */
uint32_t lnGetUs()
{    
    return (uint32_t)lnGetUs64();
}
// EOF
