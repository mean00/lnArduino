/*  
 *  This is a minimalistic wrapper to provide Arduino API on top of lnAPI
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "Arduino.h"
#include "lnArduino.h"
extern "C" uint32_t xTickCount;
extern "C" uint64_t get_timer_value();
extern "C" uint32_t SystemCoreClock ;


void pinMode(uint8_t a, uint8_t b)
{
    switch(b)
    {
        case  INPUT:         lnPinMode(a,lnFLOATING);break;
        case  OUTPUT:        lnPinMode(a,lnOUTPUT);break;
        case  INPUT_PULLUP:  lnPinMode(a,lnINPUT_PULLUP);break;            
        default:
            xAssert(0);
            break;
    }
   
}

int  digitalRead(uint8_t pin)
{
    return lnDigitalRead(pin);
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    lnDigitalWrite(pin,val);
}
uint64_t millis(void)
{
    return xTaskGetTickCount();
}
void digitalToggle(uint8_t a)
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
    lnDelayUs(wait);
}

void     noInterrupts()
{
    ENTER_CRITICAL();
}
void     interrupts()
{
    EXIT_CRITICAL();
}
typedef void (*userFuncCb)(void);

void extiInternalCb(lnPin pin,void *f)
{
    userFuncCb xfunction=(userFuncCb)f;
    xfunction();
}
void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
    lnEdge oedge;
    switch(mode)
    {
        case NONE:    oedge=LN_EDGE_NONE;break;
        case CHANGE:  oedge=LN_EDGE_BOTH;break;
        case FALLING: oedge=LN_EDGE_FALLING;break;
        case RISING:  oedge=LN_EDGE_RISING;break;
    }
    lnExtiAttachInterrupt(interruptNum, oedge, extiInternalCb, (void *)userFunc);
}
void detachInterrupt(uint8_t interruptNum)
{
    lnExtiDetachInterrupt(interruptNum);
}
