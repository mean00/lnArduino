#include "stdint.h"
extern "C"
{
#include "riscv_encoding.h"
#include "n200_timer.h"
#include "n200_func.h"
#include "gd32vf103.h"
    
#include "FreeRTOS.h"
}
#include "systemHelper.h"

extern "C" 
{
    void deadEnd(int code);
    extern void taskENTER_CRITICAL( void );
    extern void taskEXIT_CRITICAL( void );
    
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
        EXIT_CRITICAL();
    }
    void noInterrupt() 
    {
        ENTER_CRITICAL();
    }
    void vApplicationStackOverflowHook()
    {
        deadEnd(0x1001);
    }
    int _getpid()
    {
        return 0;
    }
    int _kill()
    {
        return 0;
    }
    void __cxa_pure_virtual()
    {
          deadEnd(0x2002);
    }

}



// EOF

