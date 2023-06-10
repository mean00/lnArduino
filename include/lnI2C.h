/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"

/**
 *
 * @param instance
 * @param speed
 */
class lnI2C
{
  public:
    lnI2C(int instance, int speed = 0);
    virtual ~lnI2C();
    void setSpeed(int speed);
    void setAddress(int address);
    bool write(uint32_t n, const uint8_t *data);
    bool read(uint32_t n, uint8_t *data);
    bool write(int target, uint32_t n, const uint8_t *data);
    bool multiWrite(int target, uint32_t nbSeqn, const uint32_t *seqLength, const uint8_t **data);
    bool read(int target, uint32_t n, uint8_t *data);
    bool begin(int target = 0);

  protected:
    void *cookie;
};
