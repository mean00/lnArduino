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
#include "mcp23017_internal.h"

//
//  Input button
//
//
//
/**
 * 
 * @param mcp
 * @param pin
 */

myMcpButtonInput::myMcpButtonInput(myMcp23017 *mcp, int pin) : myMcpClient(mcp)
{
    _pin=pin;
    _state=false;
    _toggle=false;
    myMcp23017 *impl=(myMcp23017 *)mcp;
    impl->registerClient(1<<pin,this);
}
/**
 * 
 * @param pins
 * @param states
 * @return 
 */
 bool myMcpButtonInput::process(int pins, int states)
 {
     int oldstate=_state;          
     bool newState=!!(states & (1<<_pin));
     noInterrupts();
     if(oldstate==true && newState==false) // falling edge
     {
            
            _toggle^=1;
            _changed=true;
     }
     _state=newState; // disable interrupts ?
     interrupts();
     return true;
 }
// EOF


