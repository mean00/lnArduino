#include "lnArduino.h"
#include "stdarg.h"
#include "lnSerial.h"

lnSerial *serial0=NULL;

static char buffer[1024];

/**
 * 
 * @param fmt
 */
void Logger(const char *fmt...)
{
    va_list va;
    va_start(va,fmt);
    vsnprintf(buffer,1022,fmt,va);
    
    buffer[1023]=0;
    serial0->transmit(strlen(buffer),(uint8_t *)buffer);
    va_end(va);    
}
/**
 * 
 */
void LoggerInit()
{
    serial0=new lnSerial(0,USART0_IRQn,USART0);
    serial0->init();  
    serial0->setSpeed(115200);
    serial0->enableTx(true);
}

