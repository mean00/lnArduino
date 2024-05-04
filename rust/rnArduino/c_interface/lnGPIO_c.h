#pragma once
#include "lnGPIO.h"
volatile uint32_t *lnGetGpioOnRegister(int port); // Bop register for port "port" with port A:0, B:1, ...
volatile uint32_t *lnGetGpioOffRegister(int port); // 
