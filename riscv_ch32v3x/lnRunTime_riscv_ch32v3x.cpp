#include "lnArduino.h"

extern void lnExtiJtagNoResetOnly();

void lnRunTimeInit()
{


}

void lnRunTimeInitPostPeripherals()
{
   // We use fake SWD, no jtag stuff lnExtiJtagNoResetOnly();
}
// EOF

