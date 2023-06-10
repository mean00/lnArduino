/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnI2C.h"
#include "lnArduino.h"
#include "lnI2C_impl.h"
#include "lnI2C_priv.h"

#define COOKIE ((lnTwoWire *)cookie)

lnI2C::lnI2C(int instance, int speed)
{
    lnPeripherals::enable((Peripherals)(pI2C0 + instance));
    lnTwoWire *tw = new lnTwoWire(instance, speed);
    cookie = (void *)tw;
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

bool lnI2C::write(uint32_t n, const uint8_t *data)
{
    return COOKIE->write(n, data);
}

bool lnI2C::read(uint32_t n, uint8_t *data)
{
    return COOKIE->read(n, data);
}

bool lnI2C::write(int target, uint32_t n, const uint8_t *data)
{
    return COOKIE->write(target, n, data);
}

bool lnI2C::multiWrite(int target, uint32_t nbSeqn, const uint32_t *seqLength, const uint8_t **data)
{
    return COOKIE->multiWrite(target, nbSeqn, seqLength, data);
}

bool lnI2C::read(int target, uint32_t n, uint8_t *data)
{
    return COOKIE->read(target, n, data);
}

bool lnI2C::begin(int target)
{
    return COOKIE->begin(target);
}

// EOF
