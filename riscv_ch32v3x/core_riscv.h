/********************************** (C) COPYRIGHT  *******************************
* File Name          : core_riscv.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : RISC-V Core Peripheral Access Layer Header File for CH32V30x
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#pragma once
#ifdef __cplusplus
 extern "C" {
#endif

#define     __I     volatile                /* defines 'read only' permissions */
#define     __IO    volatile                  /* defines 'read / write' permissions */

/* memory mapped structure for SysTick */
typedef struct
{
    volatile uint32_t CTLR;
    volatile uint32_t SR;
    volatile uint64_t CNT;
    volatile uint64_t CMP;
}SysTick_Type;
#define SysTick         ((SysTick_Type *) 0xE000F000)

extern void NVIC_SetPendingIRQ(IRQn_Type);
extern void NVIC_EnableIRQ(IRQn_Type IRQn);
extern void NVIC_SetPriority(IRQn_Type IRQn, uint8_t priority);

#ifdef __cplusplus
 };
#endif
