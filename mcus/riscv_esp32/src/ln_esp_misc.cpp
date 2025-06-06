
#include "esprit.h"
#include "lnSerial.h"
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
lnSerialTxOnly *createLnSerialTxOnly(int instance, bool dma, bool buffered)
{
    xAssert(0);
    return NULL;
}
//  EOF
