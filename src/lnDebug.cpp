/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "stdarg.h"
#include "lnSerial.h"
#define LOGGER_USE_DMA 1

extern lnSerial *serial0=NULL;

extern "C" void Logger_C(const char *fmt,...)
{
  static char buffer[128];

  va_list va;
  va_start(va,fmt);
  vsnprintf(buffer,127,fmt,va);

  buffer[127]=0;
#ifdef LOGGER_USE_DMA
  serial0->dmaTransmit(strlen(buffer),(uint8_t *)buffer);
#else
  serial0->transmit(strlen(buffer),(uint8_t *)buffer);
#endif
  va_end(va);
}


/**
 *
 * @param fmt
 */
void Logger(const char *fmt...)
{
    static char buffer[128];

    if(fmt[0]==0) return;

    va_list va;
    va_start(va,fmt);
    vsnprintf(buffer,127,fmt,va);

    buffer[127]=0;
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
  int debugUart=0;
#ifdef LN_DEBUG_UART
  debugUart=LN_DEBUG_UART;
#endif
    serial0=new lnSerial(debugUart,16); // Tx only...
    serial0->init();
    serial0->setSpeed(115200);
}
