/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "stdint.h"

#pragma clang diagnostic ignored "-Wextra"

extern "C"
{
// #include "riscv_encoding.h"
#include "FreeRTOS.h"
}
#include "systemHelper.h"

#include "lnSystemTime.h"
extern "C" void do_assert(const char *a);

/**
 */

extern "C"
{
    void deadEnd(int code);
    extern void taskENTER_CRITICAL(void);
    extern void taskEXIT_CRITICAL(void);

    uintptr_t handle_trap(uintptr_t mcause, uintptr_t sp)
    {
        deadEnd(0xffff);
        return 0;
    }

    __attribute__((interrupt)) void unhandledException(void)
    {
        deadEnd(0x1000);
    }
    void _exit(int code)
    {
        deadEnd(code + 0x2000);
        while (1)
        {
        }
    }
    void interrupts()
    {
        EXIT_CRITICAL();
    }
    void noInterrupt()
    {
        ENTER_CRITICAL();
    }
    void vApplicationStackOverflowHook(void *xTask, char *pcTaskName)
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
    int getpid(void)
    {
        do_assert("getpid");
        return 0;
    }
    int kill(int pid, int sig)
    {
        do_assert("kill");
        return 0;
    }
    int getppid(void)
    {
        do_assert("getppid");
        return 0;
    }
    void __cxa_pure_virtual()
    {
        do_assert("__cxa_pure_virtual");
    }
    uint32_t core_exception_handler(unsigned long mcause, unsigned long sp)
    {
        do_assert("core_exception_handler");
        return 0;
    }
    void abort()
    {
        do_assert("abort");
        while (1)
        {
        }
    }
    void __attribute__((noreturn)) panic(const char *fmt, ...)
    {
        do_assert(fmt);
    }
    int stderr;
}

namespace std
{
void __throw_out_of_range(const char *a) //__throw_out_of_range
{
    do_assert(a);
}
void __throw_length_error(const char *a) //__throw_out_of_range
{
    do_assert(a);
}
} // namespace std
/**

*/
void lnDelayUs(uint32_t wait)
{
    uint64_t target = lnGetUs() + wait;
    while (1)
    {
        uint64_t vw = lnGetUs();
        if (vw > target)
            return;
        __asm__("nop" ::);
    }
}

static uint32_t myTick;
extern "C" void vApplicationTickHook()
{
    // this is not atomic, ok but it is called under interrupt
    myTick++;
}

/**
 *
 * @return
 */
uint32_t lnGetMs()
{
    return myTick;
}

void lnDelay(uint32_t a);
/**
 */
void xDelay(uint32_t wait)
{
    lnDelay(wait);
}

// EOF
