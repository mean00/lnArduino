/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "stdarg.h"
#include "lnSerial.h"
//#define LOGGER_USE_DMA 1
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
#ifdef LOGGER_USE_DMA    
    serial0->dmaTransmit(strlen(buffer),(uint8_t *)buffer);    
#else
    serial0->transmit(strlen(buffer),(uint8_t *)buffer);
#endif
    va_end(va);    
}
/**
 * 
 */
void LoggerInit()
{
    serial0=new lnSerial(0);
    serial0->init();  
    serial0->setSpeed(115200);    
}

