

#pragma once
#include "lnArduino.h"
 /*
  * The usual sequence is
  * rpPio(0)
  * setSpeed()
  * configure()
  * uploadCode()
  * ...
  * execute()
  * ...
  * stop()
  * ...
  * execute()
  *
  */

class rpPIO {
public:
             rpPIO(uint32_t unit);
virtual      ~rpPIO();
        // size is in 16 bits instrucitons
        bool uploadCode(uint32_t codeSize, const uint16_t *code);
        bool execute();
        bool stop();
        bool setSpeed(uint32_t fq);
        bool configure(uint32_t startPin, uint32_t nb_pin, uint32_t start_set_pin );
protected:
        int _unit;
        int _offset;
        int _codeSize;
};

