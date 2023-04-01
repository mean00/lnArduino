#include "lnArduino.h"

extern void lnExtiJtagNoResetOnly();
extern void lnSystemTimerInit();

void lnRunTimeInit()
{


}

void lnRunTimeInitPostPeripherals()
{
   lnSystemTimerInit();
   // We use fake SWD, no jtag stuff lnExtiJtagNoResetOnly();
}
// EOF

