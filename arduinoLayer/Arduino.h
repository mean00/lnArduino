#pragma once
#include "lnArduino.h"
#include "lnGPIO.h"

void pinMode(int a, GpioMode b);
void digitalWrite(int a, bool b);
void digitalToggle(int a);

extern uint64_t millis();
extern uint64_t micros();
extern void     delay(int ms);
extern void     delayMicroseconds(int us);

#define LOW 0
#define HIGH 1
#define boolean bool
#define PROGMEM