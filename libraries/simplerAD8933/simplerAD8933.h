/**
    This is a library for the MCP23017 i2c port expander
    It is derived from the adafruit one with the following change :
        * All Axxx port are inputs
        * All Bxxx ports are outputs
        * Additional methods to easily manage rotarty encoder and input button
 * 
 * It is using ARM friendly convention i.e. using "int" as much as possible
 * (c) mean00, BSD License
 * Datasheet :
 * http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf
 * 
 */

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
#pragma once
#include <lnSPI.h>
#include <lnGPIO.h>
#include "vector"

/**
 * 
 * @param pinInterrupt
 * @param addr
 * @param w
 */
class simplerAD8933 
{
public: 
        simplerAD8933(hwlnSPIClass *spi,lnPin cs)
        {
            _spi=spi;
            _cs=cs;
        }
        ~simplerAD8933()
        {
            _spi=NULL;
            
        }        
 protected:
            virtual uint8_t   readRegister(int addr);
            virtual void      writeRegister(int addr, int value);
            hwlnSPIClass     *_spi;
            lnPin             _cs;
    
};

