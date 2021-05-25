/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnI2C.h"
#include "lnI2C_priv.h"

LN_I2C_Registers *aI2C0=(LN_I2C_Registers *)LN_I2C0_ADR;
LN_I2C_Registers *aI2C1=(LN_I2C_Registers *)LN_I2C1_ADR;

static const LN_I2C_Registers *ai2cs[2]={aI2C0,aI2C1};

struct LN_I2C_DESCRIPTOR
{
    LN_I2C_Registers *adr;
    lnPin           _scl;
    lnPin           _sda;
};

static const LN_I2C_DESCRIPTOR i2c_descritor[2]=
{
    {aI2C0,PB6,PB7},
    {aI2C1,PB10,PB11},
};

/**
 * 
 * @param instance
 * @param speed
 */
lnTwoWire::lnTwoWire(int instance, int speed)
{
    _instance=instance;
}
/**
 * 
 */
lnTwoWire::~lnTwoWire()
{
}
/**
 * 
 * @param target
 * @return 
 */
bool lnTwoWire::begin(int target)
{
    if(target)
        _targetAddress=target;
    
    // set pin
    xAssert(0);
    return false;
}

/**
 * 
 * @param speed
 */
void lnTwoWire::setSpeed(int speed)
{
    
}
/**
 * 
 * @param target
 * @param n
 * @param data
 * @return 
 */
bool lnTwoWire::write(int target, int n, uint8_t *data)
{
    return false;
}
/**
 * 
 * @param target
 * @param n
 * @param data
 * @return 
 */
bool lnTwoWire::read(int target,  int n, uint8_t *data)
{
    return false;
}
// EOF

