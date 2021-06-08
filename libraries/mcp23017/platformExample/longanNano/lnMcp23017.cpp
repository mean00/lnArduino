#include "lnMcp23017.h"
#include "lnExti.h"
/**
 * 
 * @param addr
 * @return 
 */
uint8_t   lnMcp23017::readRegister(int addr)
{
    
    
}
/**
 * 
 * @param addr
 * @param value
 */
void      lnMcp23017::writeRegister(int addr, int value)
{
    
}

void lnMcp23017::cb_(int pin, void *a)
{
   lnMcp23017 *t=(lnMcp23017*) a;
   t->interrupt();
}
/**
 * 
 * @return 
 */
bool      lnMcp23017::begin()
{
    // step 1, init MCP itself
    if(!myMcp23017::begin()) return false;
    // step2, setup input pin as interrupt
    pinMode(_irqPin,lnINPUT_PULLUP);
    lnExtiAttachInterrupt(_irqPin, LN_EDGE_FALLING, lnMcp23017::cb_,this);
    return true;
}
// Fail