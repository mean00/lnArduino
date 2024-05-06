/**
 *  Massively over engineered power supply
 *  This is the startup code that will then give control to the rust part
 
 * 
 */
#include "lnArduino.h"

extern "C" void rnLoop(void);
extern "C" void rnInit(void);

/**
 * 
 */
void setup()
{
    rnInit();
    return;
}

/**
  * 
  */
void loop()
{
    rnLoop();
    deadEnd(4);
}

extern "C" 
{
void __aeabi_unwind_cpp_pr0()
{
        xAssert(0);
}
}
// EOF
