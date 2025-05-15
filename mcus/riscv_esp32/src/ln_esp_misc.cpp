
#include "lnArduino.h"
/*
 *
 */
extern "C" void deadEnd(int code)
{
    __asm__("ebreak");
    //    lnSoftSystemReset();
}
//  EOF
