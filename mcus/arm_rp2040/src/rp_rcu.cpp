/**
 * @file    rp_serial.cpp
 * @author  mean00
 * @brief
 * @version 0.1
 * @date 2023-11-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "lnArduino.h"
#include "lnRCU.h"
#include "hardware/clocks.h"
/**

 */
uint32_t lnPeripherals::getClock(const Peripherals periph)
{
    switch(periph)
    {
        case pSYSCLOCK:
            return clock_get_hz(clk_sys);
            break;
        default:
            xAssert(0);        
            break;
    }
}
//- EOF