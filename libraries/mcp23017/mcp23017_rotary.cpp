/*************************************************** 
 This is a library for the MCP23017 i2c port expander

 These displays use I2C to communicate, 2 pins are required to
 interface
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Arduino.h"
#include "mcp23017.h"

//
//
 
 // Use the full-step state table (emits a code at 00 only)
#define R_START     0x0
#define R_CW_FINAL  0x1
#define R_CW_BEGIN  0x2
#define R_CW_NEXT   0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT  0x6
 // Directions
#define DIR_NONE    0x0
// Clockwise step.
#define DIR_CW      0x10
// Counter-clockwise step.
#define DIR_CCW     0x20
 
const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
 
 /**
  * 
  * @param mcp
  * @param pin1
  * @param pin2
  */
 myMcpRotaryEncoder::myMcpRotaryEncoder(myMcp23017 *mcp, int pin1,int pin2): myMcpClient(mcp)
 {
     _pin1=pin1;
     _pin2=pin2;
     mcp->registerClient((1<<pin1)+(1<<pin2),this);
     _state = R_START;
     _count=0;
 }
/**
 * 
 * @param pins
 * @param states
 * @return 
 */
bool myMcpRotaryEncoder::process(int pins, int states)
 {
    int s2=!!(states & (1<<_pin2));
    int s1=!!(states & (1<<_pin1));
    
    int pinstate=(s2<<1) | s1;
     
    // Determine new state from the pins and state table.
    _state = ttable[_state & 0xf][pinstate];
    // Return emit bits, ie the generated event.
    noInterrupts();
    switch(_state & 0x30)
    {
        case DIR_CW:  _count++;break;
        case DIR_CCW: _count--;break;
    }
    interrupts();
    return true;
 }

// EOF


