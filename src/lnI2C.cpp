/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnI2C.h"
#include "lnI2C_priv.h"
#include "lnI2C_impl.h"

#define COOKIE ((lnTwoWire *)cookie)

lnI2C::lnI2C(int instance, int speed)
{
    lnTwoWire *tw=new lnTwoWire(instance,speed);
    cookie=(void *)tw;
      
}
lnI2C::~lnI2C()
{
    
}
void lnI2C::setSpeed(int speed)
{
     COOKIE->setSpeed(speed);
}
void lnI2C::setAddress(int address)
{
     COOKIE->setAddress(address);
}

bool lnI2C::write(int n, uint8_t *data)
{
    return COOKIE->write(n,data);
}

bool lnI2C::read(int n, uint8_t *data)
{
    return COOKIE->read(n,data);
}

bool lnI2C::write(int target, int n, uint8_t *data)
{
    return COOKIE->write(target,n,data);
}

bool lnI2C::multiWrite(int target, int nbSeqn,int *seqLength, uint8_t **data)
{
    return COOKIE->multiWrite(target,nbSeqn,seqLength, data);
}

bool lnI2C::read(int target,  int n, uint8_t *data)
{
     return COOKIE->read(target,n,data);
}

bool lnI2C::begin(int target)
{
    return COOKIE->begin(target);
}

// EOF

