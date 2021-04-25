
extern "C"
{
#include "FreeRTOS.h"

/*
 Use freeRTOS allocator
 It is thread safe + it avoid pulling exception support from malloc & new
 
 */
void * pvPortMalloc( size_t xWantedSize );
void   vPortFree( void * pv );

void *sysAlloc(int size)
{
    return pvPortMalloc(size);
}
void sysFree(void *p)
{
    return vPortFree(p);
}

//----------------------------------
void  *malloc(size_t size)
{
    return sysAlloc(size);
}

void free(void *p)
{
    sysFree(p);
}

void *calloc(size_t n, size_t elemSize)
{
    return sysAlloc(n*elemSize);
}

}

// C++ part
void *operator new(size_t size)
{
    return sysAlloc(size);
}
void operator delete(void *p)
{
    sysFree(p);
}
void operator delete(void *p,unsigned int n)
{
    sysFree(p);
}
void operator delete[](void *p)
{
    sysFree(p);
}
// EOF