#include "lnArduino.h"

extern "C" uint64_t lnGetUs()
{  
#warning FIXME!
return xTaskGetTickCount()*1000;
}
