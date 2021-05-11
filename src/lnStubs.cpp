
#include "lnArduino.h"

extern "C" void _write() { xAssert(0);}
extern "C" void *_sbrk(ptrdiff_t incr) { xAssert(0);return 0;}

