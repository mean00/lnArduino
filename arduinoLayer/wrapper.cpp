/*  
 *  This is a minimalistic wrapper to provide Arduino API on top of lnAPI
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "Arduino.h"
extern "C" uint32_t xTickCount;
extern "C" uint64_t get_timer_value();
extern "C" uint32_t SystemCoreClock ;

void pinMode(int a, GpioMode b)
{
    lnPinMode(a,b);
}
void digitalWrite(int a, bool b)
{
    lnDigitalWrite(a,b);
}
uint64_t millis(void)
{
    return xTaskGetTickCount();
}
void digitalToggle(int a)
{
    lnDigitalToggle(a);
}
uint64_t micros(void)
{
    return (uint64_t)(get_timer_value() * (4000000.F / SystemCoreClock));
}

void delay(unsigned long dwMs)
{
    xDelay(dwMs);
}
