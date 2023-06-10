/**************************************************************************/
/*!
    @file     Adafruit_MCP4725.h
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    This is a library for the Adafruit MCP4725 breakout board
    ----> http://www.adafruit.com/products/935

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/
#pragma once
#include "lnArduino.h"
#include "lnI2C.h"

#define MCP4725_DEFAULT_I2C_ADDRESS 0x60
/**
 */
class myMCP4725
{
  public:
    myMCP4725(lnI2C &wire, int address = MCP4725_DEFAULT_I2C_ADDRESS);
    void setVoltage(int output);
    void setDefaultValue(int output); // untested

  private:
    uint8_t _i2caddr;
    lnI2C &_i2c;
};
