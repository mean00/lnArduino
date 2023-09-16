
#include "lnArduino.h"

extern "C" void _write()
{
    xAssert(0);
}
extern "C" void *_sbrk(ptrdiff_t incr)
{
    xAssert(0);
    return 0;
}
extern "C" void _putchar(char){};

#undef printf
#undef scanf

extern "C" int printf(const char *__restrict, ...)
{
    return 0;
}
extern "C" int scanf(const char *__restrict, ...)
{
    return 0;
}
