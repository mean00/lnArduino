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
    lnINPUT_FLOATING = 0,
    lnINPUT_PULLUP = 2,
    lnINPUT_PULLDOWN = 3,
    lnOUTPUT = 1,
    lnOUTPUT_OPEN_DRAIN = 4,
    lnALTERNATE_PP,
    lnALTERNATE_OD,
    lnPWM,
    lnADC_MODE,
    lnDAC_MODE,
    lnUART,
};
#define GpioMode lnGpioMode
// typedef int lnPin;
void lnPinMode(const lnPin pin, const lnGpioMode mode);
void lnDigitalWrite(const lnPin pin, bool value);
bool lnDigitalRead(const lnPin pin);
void lnDigitalToggle(const lnPin pin);
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
    void on()
    {
        *_onoff = _onbit;
    }
    void off()
    {
        *_onoff = _offbit;
    }
    void pulseLow() __attribute__((always_inline))
    {
        *_onoff = _offbit;
        *_onoff = _onbit;
    }
    void pulseHigh() __attribute__((always_inline))
    {
        *_onoff = _onbit;
        *_onoff = _offbit;
    }

  protected:
    uint32_t *_onoff;
    uint32_t _onbit, _offbit;
};

#define LN_GPIO_OUTPUT_OD_HIZ 1
#define LN_GPIO_OUTPUT_OD_GND 0

#include "lnExti.h"
// EOF
