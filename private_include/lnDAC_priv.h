#pragma once
#pragma once
#include "lnIRQ.h"
#include "lnPeripheral_priv.h"

// Each timer has channel 0,1,2,3

/**
 */
struct LN_DAC_Registersx
{
    uint32_t CTL;      // 0
    uint32_t SWT;      // 4
    uint32_t DAC0_R12DH;    // 8
    uint32_t DAC0_L12DH;    // C
    uint32_t DAC0_R8DH;     // 10
    
    uint32_t DAC1_R12DH;     // 14
    uint32_t DAC1_L12DH;     // 18
    uint32_t DAC1_R8DH;      // 1C
    
    
    uint32_t DACC_R12DH;     // 20
    uint32_t DACC_L12DH;    //  24
    uint32_t DACC_R8DH;      // 28
    uint32_t DAC0_DO;       // 2c
    uint32_t DAC1_DO;       // 30
};
/**
 */
typedef volatile LN_DAC_Registersx LN_DAC_Registers;
/**
 */

//-------------- CTL ---------

#define LN_DAC_CTL_DTSEL_SOURCE_SW (7)
#define LN_DAC_CTL_DTSEL_SOURCE_T5 (0)
#define LN_DAC_CTL_DTSEL_SOURCE_T2 (1)
#define LN_DAC_CTL_DTSEL_SOURCE_T6 (2)
#define LN_DAC_CTL_DTSEL_SOURCE_T4 (3)
#define LN_DAC_CTL_DTSEL_SOURCE_T1 (4)
#define LN_DAC_CTL_DTSEL_SOURCE_T3 (5)
#define LN_DAC_CTL_DTSEL_SOURCE_EXTI9 (6)

#define LN_DAC_CTL_ENABLE(dac)             (1<<(0+16*dac)) // dact enable
#define LN_DAC_CTL_DBOFF(dac)              (1<<(1+16*dac)) // dact enable
#define LN_DAC_CTL_DTEN(dac)               (1<<(2+16*dac)) // dact enable
#define LN_DAC_CTL_DDMAEN0(dac)            (1<<(12+16*dac)) // dact enable
#define LN_DAC_CTL_DTSEL(dac,source)       (source<<(3+16*dac)) // dact enable
#define LN_DAC_CTL_DTSEL_MASK(dac)         (7<<(3+16*dac)) // dact enable


//-------------- SWT ---------

#define LN_DAC_SWT_ENABLE(dac)             (1<<(dac)) // 

// EOF


