
#include "lnArduino.h"
#include "lnSystemTimer_priv.h"
#include "lnPeripheral_priv.h"


LN_SYSTIMER *lnSysTimer=(LN_SYSTIMER *)LN_SYS_TIMER_ADR;
LN_SYSTIMER64 *lnSysTimer64=(LN_SYSTIMER64 *)LN_SYS_TIMER_ADR;
volatile uint8_t *lnSysTimer_msip=(volatile uint8_t *)(LN_SYS_TIMER_ADR+0xFFC);
#define  increment (SystemCoreClock / (4*configTICK_RATE_HZ))

/**
 *  This should be called with interrupt disabled
 */
extern "C" void lnSystemTimerInit()
{
    lnSysTimer64->MTIME64=0;
    lnSysTimer64->MTIMECMP64=increment;    
    
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
extern "C" void lnSystemTimerClearSwInterrupt()
{   
   *lnSysTimer_msip=0;
}



