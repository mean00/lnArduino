#include "stdint.h"
//
extern "C"
{
#include "SEGGER_RTT.h"
}
/*

*/
void rttLoggerFunction(int n, const char *data)
{
    while (n)
    {
        int done = SEGGER_RTT_Write(0, data, n);
        if (done < 0)
            return;
        n -= done;
        data += n;
    }
}
// EOF
