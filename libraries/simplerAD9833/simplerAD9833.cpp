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
#include "simplerAD9833.h"
#include "simplerAD9833_internal.h"
/**
 * 
 * @param spi
 * @param cs
 */
simplerAD9833::simplerAD9833(hwlnSPIClass *spi,lnPin cs)
{
    _spi=spi;
    _cs=cs;
}
/**
 * 
 */
simplerAD9833::~simplerAD9833()
{
    _spi=NULL;            
}        
/**
 * 
 * @param addr
 * @return 
 */
int   simplerAD9833::readRegister(int addr)
{
    return 0;
}
/**
 * 
 * @param addr
 * @param value
 */
void simplerAD9833::writeRegister(int addr, int value)
{

}
    
// EOF


