#include "lnArduino.h"
#include "lnSCB_arm_priv.h"

static  LN_SCB_Registers *aSCB=(LN_SCB_Registers *)0xE000ED00;
extern void lnExtiSWDOnly();
/**
 * 
 */
void lnRunTimeInit()
{
    __asm__("cpsid if    \n" );
    aSCB->VTOR = 0;
}
/**
 * 
 * @return 
 */
void lnRunTimeInitPostPeripherals()
{
    // Init
    lnExtiSWDOnly();
}
// EOF
