/*
 *  This is a minimalistic wrapper to provide Arduino API on top of lnAPI
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "Wire.h"
#include "Arduino.h"
#include "lnArduino.h"

/**
 *
 * @param instance
 */
TwoWire::TwoWire(int instance)
{
    xAssert(0);
}
/**
 *
 */
TwoWire::~TwoWire()
{
    xAssert(0);
}
/**
 *
 */
void TwoWire::begin()
{
    xAssert(0);
}
/**
 *
 * @param
 */
void TwoWire::begin(uint8_t)
{
    xAssert(0);
}
/**
 *
 * @param
 */
void TwoWire::begin(int)
{
    xAssert(0);
}
/**
 *
 */
void TwoWire::end()
{
    xAssert(0);
}
/**
 *
 * @param
 */
void TwoWire::setClock(uint32_t)
{
    xAssert(0);
}
/**
 *
 * @param
 */
void TwoWire::beginTransmission(uint8_t)
{
    xAssert(0);
}
/**
 *
 * @param
 */
void TwoWire::beginTransmission(int)
{
    xAssert(0);
}
/**
 *
 * @return
 */
uint8_t TwoWire::endTransmission(void)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @return
 */
uint8_t TwoWire::endTransmission(uint8_t)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @param
 * @return
 */
uint8_t TwoWire::requestFrom(uint8_t, uint8_t)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @param
 * @param
 * @return
 */
uint8_t TwoWire::requestFrom(uint8_t, uint8_t, uint8_t)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 * @return
 */
uint8_t TwoWire::requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @param
 * @return
 */
uint8_t TwoWire::requestFrom(int, int)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @param
 * @param
 * @return
 */
uint8_t TwoWire::requestFrom(int, int, int)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @return
 */
size_t TwoWire::write(uint8_t)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @param
 * @param
 * @return
 */
size_t TwoWire::write(const uint8_t *, size_t)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @return
 */
int TwoWire::available(void)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @return
 */
int TwoWire::read(void)
{
    xAssert(0);
    return -1;
}
/**
 *
 * @return
 */
int TwoWire::peek(void)
{
    xAssert(0);
    return -1;
}
/**
 *
 */
void TwoWire::flush(void)
{
    xAssert(0);
}
/**
 *
 * @param
 */
void TwoWire::onReceive(void (*)(int))
{
    xAssert(0);
}
/**
 *
 * @param
 */
void TwoWire::onRequest(void (*)(void))
{
    xAssert(0);
}

// EOF
