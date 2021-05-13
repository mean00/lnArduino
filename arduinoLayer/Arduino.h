#pragma once
#include "lnArduino.h"
#include "lnGPIO.h"

void pinMode(int a, GpioMode b);
void digitalWrite(int a, bool b);
extern uint64_t millis();
extern uint64_t micros();
extern void     delay(int ms);
