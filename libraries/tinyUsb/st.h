/**
  ******************************************************************************
  * @file    dcd_stm32f0_pvt_st.h
  * @brief   DCD utilities from ST code
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  * <h2><center>&copy; parts COPYRIGHT(c) N Conrad</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  **********/

// This file contains source copied from ST's HAL, and thus should have their copyright statement.

// PMA_LENGTH is PMA buffer size in bytes.
// On 512-byte devices, access with a stride of two words (use every other 16-bit address)
// On 1024-byte devices, access with a stride of one word (use every 16-bit address)

#ifndef PORTABLE_ST_STM32F0_DCD_STM32F0_FSDEV_PVT_ST_H_
#define PORTABLE_ST_STM32F0_DCD_STM32F0_FSDEV_PVT_ST_H_

#if defined(STM32F042x6) || \
    defined(STM32F070x6) || defined(STM32F070xB) || \
    defined(STM32F072xB) || \
    defined(STM32F078xx)
  #include "stm32f0xx.h"
  #define PMA_LENGTH (1024u)
  // F0x2 models are crystal-less
  // All have internal D+ pull-up
  // 070RB:    2 x 16 bits/word memory     LPM Support, BCD Support
  // PMA dedicated to USB (no sharing with CAN)

#elif defined(STM32F1_FSDEV)
  #include "stm32f1xx.h"
  #define PMA_LENGTH (512u)
  // NO internal Pull-ups
  //         *B, and *C:    2 x 16 bits/word

  // F1 names this differently from the rest
  #define USB_CNTR_LPMODE   USB_CNTR_LP_MODE

#elif defined(STM32F302xB) || defined(STM32F302xC) || \
      defined(STM32F303xB) || defined(STM32F303xC) || \
      defined(STM32F373xC)
  #include "stm32f3xx.h"
  #define PMA_LENGTH (512u)
  // NO internal Pull-ups
  //         *B, and *C:    1 x 16 bits/word
  // PMA dedicated to USB (no sharing with CAN)

#elif defined(STM32F302x6) || defined(STM32F302x8) || \
      defined(STM32F302xD) || defined(STM32F302xE) || \
      defined(STM32F303xD) || defined(STM32F303xE)
  #include "stm32f3xx.h"
  #define PMA_LENGTH (1024u)
  // NO internal Pull-ups
  // *6, *8, *D, and *E:    2 x 16 bits/word     LPM Support
  // When CAN clock is enabled, USB can use first 768 bytes ONLY.

#elif CFG_TUSB_MCU == OPT_MCU_STM32L0
  #include "stm32l0xx.h"
  #define PMA_LENGTH (1024u)

#elif CFG_TUSB_MCU == OPT_MCU_STM32L1
  #include "stm32l1xx.h"
  #define PMA_LENGTH (512u)

#elif CFG_TUSB_MCU == OPT_MCU_STM32G4
  #include "stm32g4xx.h"
  #define PMA_LENGTH (1024u)

#else
  #error You are using an untested or unimplemented STM32 variant. Please update the driver.
  // This includes L1x0, L1x1, L1x2, L4x2 and L4x3, G1x1, G1x3, and G1x4
#endif

// For purposes of accessing the packet
#if ((PMA_LENGTH) == 512u)
  #define PMA_STRIDE  (2u)
#elif ((PMA_LENGTH) == 1024u)
  #define PMA_STRIDE  (1u)
#endif


// This checks if the device has "LPM"
#if defined(USB_ISTR_L1REQ)
#define USB_ISTR_L1REQ_FORCED (USB_ISTR_L1REQ)
#else
#define USB_ISTR_L1REQ_FORCED ((uint16_t)0x0000U)
#endif

#define USB_ISTR_ALL_EVENTS (USB_ISTR_PMAOVR | USB_ISTR_ERR | USB_ISTR_WKUP | USB_ISTR_SUSP | \
     USB_ISTR_RESET | USB_ISTR_SOF | USB_ISTR_ESOF | USB_ISTR_L1REQ_FORCED )

// Number of endpoints in hardware
#define STFSDEV_EP_COUNT (8u)

#endif /* PORTABLE_ST_STM32F0_DCD_STM32F0_FSDEV_PVT_ST_H_ */
