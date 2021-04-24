#include "stdint.h"
extern "C"
{
#include "riscv_encoding.h"
#include "n200_timer.h"
#include "n200_func.h"
#include "gd32vf103.h"
    
#include "FreeRTOS.h"
}


extern "C" 
{
    void deadEnd(int code);
    extern void vPortExitCritical( void );
    extern void vPortEnterCritical( void );
    
    __attribute__( ( interrupt ) )  void unhandledException( void ) 
    {
        deadEnd(0x1000);
    }
    void _exit(int code)
    {
        deadEnd(code+0x2000);
        while(1) {}
    }
    void interrupts()
    {
        vPortExitCritical();
    }
    void noInterrupt() 
    {
        vPortEnterCritical();
    }

}
void deadEnd(int code)
{
    // No interrrupt
    noInterrupt();
    while(1)
    {
        // blink red light...
        
    }
}



// EOF

