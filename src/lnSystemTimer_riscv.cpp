
#include "lnArduino.h"
#include "lnSystemTimer_priv.h"
#include "lnPeripheral_priv.h"

#define  increment (SystemCoreClock / (4*configTICK_RATE_HZ))

LN_SYSTIMER *lnSysTimer=(LN_SYSTIMER *)LN_SYS_TIMER_ADR;
LN_SYSTIMER64 *lnSysTimer64=(LN_SYSTIMER64 *)LN_SYS_TIMER_ADR;
volatile uint8_t *lnSysTimer_msip=(volatile uint8_t *)(LN_SYS_TIMER_ADR+0xFFC);
static uint64_t tickPerUs16=1;

/**
 *  This should be called with interrupt disabled
 */
extern "C" void lnSystemTimerInit()
{
    lnSysTimer64->MTIME64=0;
    lnSysTimer64->MTIMECMP64=increment;    
    // number of ticks for a duration of 1 us
    tickPerUs16=((SystemCoreClock*4))/(1000*1000); // *16/4

    
}
/**
 * 
 */
extern "C" void lnSystemTimerTick()
{   
    uint64_t now = lnSysTimer64->MTIMECMP64;
    now += increment;
    
    // The original freeRTOS riscv port makes sure not to have
    // a temporary wrap value
    lnSysTimer->MTIMECMP_LO=~0;
    lnSysTimer->MTIMECMP_HI=now>>32;
    lnSysTimer->MTIMECMP_LO=now&0xffffffffUL;
    
}

/**
 * 
 */
extern "C" void lnSystemTimerTriggerSwInterrupt()
{   
   *lnSysTimer_msip=1;
}
/**
 * 
 */
extern "C" void lnSystemTimerClearSwInterrupt()
{   
   *lnSysTimer_msip=0;
}

/**
 * 
 * @return 
 */
extern "C" uint64_t lnGetUs()
{    
    uint32_t high;
    uint32_t low;


    while(1)
    {
        high= lnSysTimer->MTIME_HI;
        low=  lnSysTimer->MTIME_LO;
        uint32_t high2=lnSysTimer->MTIME_HI;
        if(high==high2)
        {
            break;
        }
    }
    uint64_t tick=(((uint64_t)high)<<32)+low;
    // convert tick to us
    tick=(tick*16)/tickPerUs16;
    return tick;    
}

// EOF
