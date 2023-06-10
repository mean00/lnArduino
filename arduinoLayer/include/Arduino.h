#pragma once
#include "lnArduino.h"
#include "lnGPIO.h"

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define NONE 0
#define CHANGE 1
#define FALLING 2
#define RISING 3

// -- Pin API --
extern void pinMode(uint8_t pin, uint8_t b);
extern void digitalWrite(uint8_t pin, uint8_t val);
extern void digitalToggle(uint8_t pin);
extern int digitalRead(uint8_t pin);
extern int analogRead(uint8_t pin);
extern void analogWrite(uint8_t pin, int val);
extern void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode);
extern void detachInterrupt(uint8_t interruptNum);

// --

extern uint64_t millis();
extern uint64_t micros();
extern void delay(int ms);
extern void delayMicroseconds(int us);
extern void noInterrupts();
extern void interrupts();
#define LOW 0
#define HIGH 1
#define boolean bool
#define PROGMEM

#define byte uint8_t
