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
#include <Arduino.h>
#include "vector"
class myMcpClient;
/**
 * 
 * @param pinInterrupt
 * @param addr
 * @param w
 */
class myMcp23017 
{
public:
  
  
        /**
         * \fn digitalWrite
         * @param pin 0..7 matching b0...b7
         * @param value
         */
  virtual void              digitalWrite(int pin, bool onoff);
          void              registerClient(int mask, myMcpClient *client);
          virtual bool      begin();
          
 protected:
            virtual uint8_t   readRegister(int addr)=0;
            virtual void      writeRegister(int addr, int value)=0;
            void              interrupt();
  
            myMcp23017(int address)
            {
              _address=address;
            }
            
            int      _address;
            std::vector<myMcpClient *> clients;
            int      inputMask;
            int      PortALatch;
            int      PortBValue;
            bool     changed;
            xBinarySemaphore _sem;

            static void task(void *a);
                   void run();
                   void process();
};
/**
 */
class myMcpClient
{
public:
        myMcpClient(myMcp23017 *m)
        {
          mcp=m;
          mask=0;
        }        
        virtual bool process(int pins,int state)=0;
protected:
        myMcp23017 *mcp;
public:
        int mask;
};

/**
 */
class myMcpButtonInput : public myMcpClient
{
public:
        myMcpButtonInput(myMcp23017 *mcp, int pin) ;
        bool process(int pin, int state);
        bool state()
        {          
          return _toggle;
        }
        bool changed()
        {
          bool r=_changed;
          _changed=false;
          return r;
        }
protected:
        bool _toggle;
        int  _pin;
        bool _state;
        bool _changed;
  
};


/**
 */
class myMcpRotaryEncoder : public myMcpClient
{
public:
        myMcpRotaryEncoder(myMcp23017 *mcp, int pin1,int pin2) ;
        bool process(int pin, int state);
        int count()
        {
          noInterrupts();
          int x= _count;
          _count=0;
          interrupts();
          return x;
        }
protected:
        int  _pin1,_pin2;
        int  _count;
        int  _state;
};

