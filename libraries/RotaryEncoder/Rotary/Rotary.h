/*
 * Rotary encoder library for Arduino.
 */

#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"

class Rotary
{
  public:
    Rotary(char, char);
    unsigned char process();
    void begin(bool pullup=true);
  private:
    unsigned char state;
    unsigned char pin1;
    unsigned char pin2;
};

#endif
 
