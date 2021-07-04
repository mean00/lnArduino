/*  
 *  This is a minimalistic wrapper to provide Arduino API on top of lnAPI
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "Arduino.h"
extern "C" uint32_t xTickCount;
extern "C" uint64_t get_timer_value();
extern "C" uint32_t SystemCoreClock ;
extern "C" uint64_t lnGetUs();

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
/**
 * @brief micros
 * @return
 */
uint64_t micros(void)
{
    return lnGetUs();
}

void delay(int  dwMs)
{
    xDelay(dwMs);
}
/**
 * 
 * @param wait
 */
void delayMicroseconds(int wait)
{
    uint64_t target=lnGetUs()+wait;
    while(1)
    {
        uint64_t vw=lnGetUs();
        if(vw>target) 
            return;
        __asm__("nop"::);
    }
    
}

void     noInterrupts()
{
    ENTER_CRITICAL();
}
void     interrupts()
{
    EXIT_CRITICAL();
}
