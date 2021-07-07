/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

struct LN_SCB_Registersx
{
    uint32_t CPUID;
    uint32_t ICSR;
    uint32_t VTOR;
};

typedef volatile LN_SCB_Registersx LN_SCB_Registers;



