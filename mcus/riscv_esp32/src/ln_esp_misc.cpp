
#include "lnArduino.h"
/*
 *
 */
extern "C" void deadEnd(int code)
{
    __asm__("ebreak");
    //    lnSoftSystemReset();
}
/**
 * 
 * 
 */
extern "C" void _putchar(char c)
{
    xAssert(0);
}

//  EOF
