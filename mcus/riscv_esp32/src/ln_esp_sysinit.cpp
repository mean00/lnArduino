#include "lnArduino.h"
#include "lnDebug.h"
//
#undef printf
//
extern void LoggerInitMutex();
// Custom printf function for ESP, prints 'n' characters from 'data' followed by a newline
void espPrintf(int n, const char *data)
{
    printf("%.*s\n", n, data);
}

// System initialization function for ESP
void lnEspSysInit()
{
    LoggerInitMutex(); 
    setLogger(espPrintf); // Set the logger function to espPrintf
}
