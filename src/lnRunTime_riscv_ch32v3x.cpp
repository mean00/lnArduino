#include "lnArduino.h"

extern void lnExtiJtagNoResetOnly();

void lnRunTimeInit()
{


}

void lnRunTimeInitPostPeripherals()
{
    lnExtiJtagNoResetOnly();
}
 const unsigned short int *_ctype_b;
// EOF

