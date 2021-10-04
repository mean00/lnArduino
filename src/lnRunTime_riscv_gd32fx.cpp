#include "lnArduino.h"

extern void lnExtiJtagNoResetOnly();

void lnRunTimeInit()
{


}

void lnRunTimeInitPostPeripherals()
{
    lnExtiJtagNoResetOnly();
}

// EOF

