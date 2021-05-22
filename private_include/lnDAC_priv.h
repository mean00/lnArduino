/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once

struct LN_DAC_Registersx
    {
        uint32_t CTL;        // 0
        uint32_t SWT;        // 4
        uint32_t DAC0_R12DH; // 8
        uint32_t DAC0_L12DH; // C
        uint32_t DAC0_R8DH;  // 10
        uint32_t DAC1_R12DH; // 14
        uint32_t DAC1_L12DH; // 18
        uint32_t DAC1_R8DH;  // 1C
        uint32_t DACC_R12DH; // 20
        uint32_t DACC_L12DH; // 24
        uint32_t DACC_R8DH;  // 28
        uint32_t DAC0_DO;    // 2C
        uint32_t DAC1_DO;    // 30

        uint32_t empty;
    };
    
typedef volatile LN_DAC_Registersx LN_DAC_Registers;    
    
//----- CTL------------
#define LN_DAC_CTL_ENABLE(x)            (1<<(0+x*16))
#define LN_DAC_CTL_DBOFF(x)             (1<<(1+x*16))
#define LN_DAC_CTL_DTEN(x)              (1<<(2+x*16))
#define LN_DAC_CTL_NOISE_WIDTH_MASK(x)  (0xf<<(8+x*16))
#define LN_DAC_CTL_NOISE_MODE_MASK(x)   (0x3<<(6+x*16))
#define LN_DAC_CTL_DTSELF_MASK(x)       (0x7<<(3+x*16))
#define LN_DAC_CTL_DMAEN(x)             (1<<(12+x*16))
#define LN_DAC_CTL_NOISE_MODE_VAL(x,y)  (y<<(6+x*16))
#define LN_DAC_CTL_NOISE_WIDTH_VAL(x,y) (y<<(8+x*16))

//- SWT
#define LN_DAC_SWT_SWTR0                (1<<(0))
#define LN_DAC_SWT_SWTR1                (1<<(1))
#define LN_DAC_SWT_SWTR(x)                (1<<(x))


// DAC0 =>DMA1 CHANNEL2 PA4
// DAC1 =>DMA1 CHANNEL3 PA5
// /!\ Conflict with SPI 0 !
