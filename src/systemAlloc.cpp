/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

extern "C"
{
#include "FreeRTOS.h"
    void do_assert(const char *a);
    /*
     Use freeRTOS allocator
     It is thread safe + it avoid pulling exception support from malloc & new

     */
    void *pvPortMalloc(size_t xWantedSize);
    void vPortFree(void *pv);

    void *sysAlloc(int size)
    {
        void *m = pvPortMalloc(size);
        if (!m)
            do_assert(0);
        return m;
    }
    void sysFree(void *p)
    {
        return vPortFree(p);
    }

    //----------------------------------
    void *malloc(size_t size)
    {
        return sysAlloc(size);
    }

    void free(void *p)
    {
        sysFree(p);
    }
    extern "C" void *memset(void *s, int c, size_t n);
    void *calloc(size_t n, size_t elemSize)
    {
        int total = n * elemSize;
        void *xout = sysAlloc(total);
        if (!xout)
            return NULL;
        memset(xout, 0, total);
        return xout;
    }
}

// C++ part
void *operator new(size_t size)
{
    return sysAlloc(size);
}
void *operator new[](size_t size)
{
    return sysAlloc(size);
}

void operator delete(void *p)
{
    sysFree(p);
}
void operator delete(void *p, unsigned int n)
{
    sysFree(p);
}
void operator delete[](void *p)
{
    sysFree(p);
}
// EOF