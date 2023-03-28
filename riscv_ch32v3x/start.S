/********************************** (C) COPYRIGHT *******************************
* File Name          : startup_ch32v30x_D8C.s
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : CH32V307-CH32V305 vector table for eclipse toolchain.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

	.section	.init,"ax",@progbits
	.global	_start	    
	.align	1
	.extern start_c
_start:
	j	handle_reset
	.align  1
_vector_base:
	.option norvc;
	.word   _start
	.section	.text.handle_reset,"ax",@progbits	
	.align	1
handle_reset:
.option push 
.option	norelax 
	la gp, __global_pointer$
.option	pop 
1:
	la sp, _sp 
2:
	/* Load data section from flash to RAM */
	la a0, _data_lma
	la a1, _data
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
	/* Clear bss section */
	la a0, __bss_start
	la a1, _end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
	li t0, 0x1f
	csrw 0xbc0, t0
	J start_c
	mret


