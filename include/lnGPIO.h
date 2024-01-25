/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"

/**
 */
#include "lnGPIO_pins.h"
/**
 */
enum lnGpioMode
{
    lnFLOATING = 0,
    lnINPUT_FLOATING = 1,
    lnINPUT_PULLUP = 2,
    lnINPUT_PULLDOWN = 3,
    lnOUTPUT = 4,
    lnOUTPUT_OPEN_DRAIN = 5,
    lnALTERNATE_PP,
    lnALTERNATE_OD,
    lnPWM,
    lnADC_MODE,
    lnDAC_MODE,
    lnUART,
    lnSPI,
};
#define GpioMode lnGpioMode
// typedef int lnPin;
void lnPinMode(const lnPin pin, const lnGpioMode mode, const int speedInMhz = 0);
void lnDigitalWrite(const lnPin pin, bool value);
bool lnDigitalRead(const lnPin pin);
void lnDigitalToggle(const lnPin pin);
void lnOpenDrainClose(const lnPin pin, const bool close); // if true, the open drain is passing, else it is hiz

volatile uint32_t *lnGetGpioToggleRegister(int port); // Bop register for port "port" with port A:0, B:1, ...
volatile uint32_t *lnGetGpioDirectionRegister(
    int port); // Direction register for the bit 0..7 of port "port" , A=0, B=1, ...
volatile uint32_t *lnGetGpioValueRegister(int port); // Bit value for LOW bits of port "port"
uint32_t lnReadPort(int port);

void lnRemapTimerPin(int timer);

/**
 *
 * @param p
 */
class lnFastIO
{
  public:
    lnFastIO(lnPin p);
    void on() volatile
    {
        *_onoff = _onbit;
    }
    void off() volatile
    {
        *_onoff = _offbit;
    }
    void pulseLow() volatile __attribute__((always_inline))
    {
        *_onoff = _offbit;
        *_onoff = _onbit;
    }
    void pulseHigh() volatile __attribute__((always_inline))
    {
        *_onoff = _onbit;
        *_onoff = _offbit;
    }

  protected:
    volatile uint32_t *_onoff;
    volatile uint32_t _onbit, _offbit;
};

#include "lnExti.h"
// EOF
