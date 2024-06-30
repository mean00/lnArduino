

#pragma once
#include "lnArduino.h"


class rpPIO {
public:
             rpPIO(uint32_t unit);
virtual      ~rpPIO();
        bool uploadCode(uint32_t codeSize, const uint8_t *code);
        bool execute();
        bool stop();
protected:
        int _unit;
        int _offset;
        int _codeSize;
};

