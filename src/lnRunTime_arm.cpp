#include "lnArduino.h"
#include "lnSCB_arm_priv.h"
#include "lnRCU_priv.h"

static  LN_SCB_Registers *aSCB=(LN_SCB_Registers *)0xE000ED00;
extern void lnExtiSWDOnly();
extern LN_RCU *arcu;
/**
 * 
 */
void lnRunTimeInit()
{
    __asm__("cpsid if    \n" );
    aSCB->VTOR = 0;
    arcu->RSTCLK=1<<24; // reset all reset flags
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
