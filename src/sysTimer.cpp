#include "stdint.h"
extern "C"
{
#include "riscv_encoding.h"
#include "n200_timer.h"
#include "n200_func.h"
#include "gd32vf103.h"
}


extern "C" void _exit(int code);
extern "C"  __attribute__( ( interrupt ) )  void unhandledException( void ) 
{
    _exit(0);
}





// EOF

