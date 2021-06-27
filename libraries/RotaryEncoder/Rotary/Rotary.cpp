/* Rotary encoder handler for arduino.
 *
 * Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
 * Contact: bb@cactii.net
 *
 */

#include "Arduino.h"
#include "Rotary.h"

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */

/*
 * Constructor. Each arg is the pin number for each encoder contact.
 */
Rotary::Rotary(char _pin1, char _pin2) {
  // Assign variables.
  pin1 = _pin1;
  pin2 = _pin2;
  // Initialise state.
  state = R_START;
}

void Rotary::begin(bool pullup) {

#if 0
  if (pullup){
    // Enable weak pullups
    pinMode(pin1,INPUT_PULLUP);
    pinMode(pin2,INPUT_PULLUP);
  }else{
    // Set pins to input.
    pinMode(pin1, INPUT);
    pinMode(pin2, INPUT);
  }
#endif
}

unsigned char Rotary::process() {
  // Grab state of input pins.
  unsigned char pinstate = (lnDigitalRead(pin2) << 1) | lnDigitalRead(pin1);
 
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  return state & 0x30;
}
