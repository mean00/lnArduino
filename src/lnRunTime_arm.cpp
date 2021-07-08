#include "lnArduino.h"


extern void lnExtiSWDOnly();
/**
 * 
 */
void lnRunTimeInit()
{
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
