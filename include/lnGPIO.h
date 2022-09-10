/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

#define PPINS(a,val) P##a##0=val,P##a##1,P##a##2,P##a##3,P##a##4,P##a##5,P##a##6,P##a##7,P##a##8,P##a##9,P##a##10,P##a##11,P##a##12,P##a##13,P##a##14,P##a##15,
/**
 */
enum lnPin
{
    PPINS(A,0)
    PPINS(B,16)
    PPINS(C,32)
    PPINS(D,48)
    PPINS(E,64)
    NoPin=-1
};

/**
 */
enum lnGpioMode
{
    lnFLOATING=0,
    lnINPUT_FLOATING=0,
    lnINPUT_PULLUP=2,
    lnINPUT_PULLDOWN=3,
    lnOUTPUT=1,
    lnOUTPUT_OPEN_DRAIN=4,
    lnALTERNATE_PP,
    lnALTERNATE_OD,
    lnPWM,
    lnADC_MODE,
    lnDAC_MODE
};
#define GpioMode lnGpioMode
//typedef int lnPin;
void lnPinMode(const lnPin pin, const lnGpioMode mode);
void lnDigitalWrite(const lnPin pin, bool value);
bool lnDigitalRead(const lnPin pin);
void lnDigitalToggle(const lnPin pin);
volatile uint32_t *lnGetGpioToggleRegister(int port); // Bop register for port "port" with port A:0, B:1, ...
volatile uint32_t *lnGetGpioDirectionRegister(int port); // Direction register for the bit 0..7 of port "port" , A=0, B=1, ...
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
    void on() {*_onoff=_onbit;}
    void off() {*_onoff=_offbit;}
    void pulseLow() __attribute__((always_inline)) { *_onoff=_offbit;*_onoff=_onbit;}
    void pulseHigh() __attribute__((always_inline)) { *_onoff=_onbit ;*_onoff=_offbit;}
protected:
    uint32_t *_onoff;
    uint32_t  _onbit,_offbit;
};

#include "lnExti.h"
// EOF
