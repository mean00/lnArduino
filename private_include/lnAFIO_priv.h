/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once

struct LN_AFIOx
{
    uint32_t EC;
    uint32_t PCF0;
    uint32_t EXTISS[4];
    uint32_t dummy;
    uint32_t PCF1;
};

typedef volatile LN_AFIOx LN_AFIO;    


#define LN_AFIO_PCF0_SWJ_SET(x) (x<<24)
#define LN_AFIO_PCF0_SWJ_MASK (~(7<<24))

