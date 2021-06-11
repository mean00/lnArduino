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


/**
 */
void myMcp23017::task(void *a)
{
    myMcp23017 *t=(myMcp23017 *)a;
    t->run();
    
}
/**
 */
void myMcp23017::run()
{
    while(1)
    {
        _sem.take();
        process();
    }
}
/**
 * \fn init
 * \brief set default sane value for most configuration
 */

bool  myMcp23017::begin() 
{

    // set defaults:
    // "A" Pins are all input, "B" pins are all outputs
    writeRegister(MCP23017_IODIRA,0xff);
    // and all pullup
    writeRegister(MCP23017_GPPUA,0xff);
    writeRegister(MCP23017_OLATA,0xff);
    PortALatch=0xff;


    // B are all outputs
    writeRegister(MCP23017_IODIRB,0);
    // set to gnd by default
    writeRegister(MCP23017_OLATB,0);
    PortBValue=0;
    inputMask=0;
    // All interrupt on change for all A
    writeRegister(MCP23017_GPINTENA,inputMask);
    // And use previous value
    writeRegister(MCP23017_INTCONA,0);
    // Clear pending interrupts if any
    readRegister(MCP23017_GPIOA);
    readRegister(MCP23017_GPIOB);
    changed=false;       
    xTaskCreate(myMcp23017::task,"mcp23017",500,this,2,NULL);    
    return true;
}



/**
 * 
 * @param pin
 * @param onoff
 */
void      myMcp23017::digitalWrite(int pin, bool onoff)
{
    int msk=1<<pin;
    if(onoff) 
        PortBValue|=msk;
    else 
        PortBValue&=~msk;
    writeRegister(MCP23017_OLATB,PortBValue);     
}

/**
 * 
 */
void myMcp23017::process()
{
    // MCP23017_INTCAPA = value when interrupt occurred
    // MCP23017_GPIOA = value now
    int newValue=readRegister(MCP23017_INTCAPA) ; //MCP23017_INTCAPA);
    if(newValue!=PortALatch)
    {
        int changed=newValue^PortALatch;
        int n=clients.size();
        for(int i=0;i<n;i++)
        {
            int masked=clients[i]->mask & changed;
            if(masked)
            {
                clients[i]->process(masked,newValue);
                changed &=~masked;
            }
            if(!changed)
                break;
        }
    }
    PortALatch=newValue;
}

/**
 * 
 * @param mask
 * @param client
 */
void      myMcp23017::registerClient(int mask, myMcpClient *client)
{

    clients.push_back(client);
    client->mask=mask;
    inputMask|=mask; // Allow interrupts on those pins
}
/**
 * 
 */
void myMcp23017::interrupt()
{
    _sem.give();
}
// EOF


