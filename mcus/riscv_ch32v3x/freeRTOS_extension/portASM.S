/*
 * FreeRTOS Kernel V10.4.6
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code which tailors the port to a specific RISC-V chip:
 *
 * + The code that is common to all RISC-V chips is implemented in
 *   FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S.  There is only one
 *   portASM.S file because the same file is used no matter which RISC-V chip is
 *   in use.
 *
 * + The code that tailors the kernel's RISC-V port to a specific RISC-V
 *   chip is implemented in freertos_risc_v_chip_specific_extensions.h.  There
 *   is one freertos_risc_v_chip_specific_extensions.h that can be used with any
 *   RISC-V chip that both includes a standard CLINT and does not add to the
 *   base set of RISC-V registers.  There are additional
 *   freertos_risc_v_chip_specific_extensions.h files for RISC-V implementations
 *   that do not include a standard CLINT or do add to the base set of RISC-V
 *   registers.
 *
 * CARE MUST BE TAKEN TO INCLDUE THE CORRECT
 * freertos_risc_v_chip_specific_extensions.h HEADER FILE FOR THE CHIP
 * IN USE.  To include the correct freertos_risc_v_chip_specific_extensions.h
 * header file ensure the path to the correct header file is in the assembler's
 * include path.
 *
 * This freertos_risc_v_chip_specific_extensions.h is for use on RISC-V chips
 * that include a standard CLINT and do not add to the base set of RISC-V
 * registers.
 *
 */
#if __riscv_xlen == 64
	#define portWORD_SIZE 8
	#define store_x sd
	#define load_x ld
#elif __riscv_xlen == 32
	#define store_x sw
	#define load_x lw
	#define portWORD_SIZE 4
#else
	#error Assembler did not define __riscv_xlen
#endif

/* we define float registers at specific_extensions.h, V30x support hardware floating point unit
can be enable by defined ARCH_FPU*/
#if ARCH_FPU
	#include "freertos_risc_v_chip_specific_extensions_fpu.h"
#else
	#include "freertos_risc_v_chip_specific_extensions_no_fpu.h"
#endif


/* Only the standard core registers are stored by default.  Any additional
registers must be saved by the portasmSAVE_ADDITIONAL_REGISTERS and
portasmRESTORE_ADDITIONAL_REGISTERS macros - which can be defined in a chip
specific version of freertos_risc_v_chip_specific_extensions.h.  See the notes
at the top of this file. */
#define portCONTEXT_SIZE ( 28 * portWORD_SIZE )
#define portHEADER_SIZE ( 2 * portWORD_SIZE )

.global xPortStartFirstTask
.global SW_Handler
.global pxPortInitialiseStack

.extern pxCurrentTCB
.extern vTaskSwitchContext
.extern xISRStackTop
/*-----------------------------------------------------------*/
.align 8

SW_Handler:
	addi sp, sp, -portCONTEXT_SIZE
	store_x x1, 0 * portWORD_SIZE( sp )
	store_x x5, 1 * portWORD_SIZE( sp )
	store_x x6, 2 * portWORD_SIZE( sp )
	store_x x7, 3 * portWORD_SIZE( sp )
	store_x x8, 4 * portWORD_SIZE( sp )
	store_x x9, 5 * portWORD_SIZE( sp )
	store_x x10, 6 * portWORD_SIZE( sp )
	store_x x11, 7 * portWORD_SIZE( sp )
	store_x x12, 8 * portWORD_SIZE( sp )
	store_x x13, 9 * portWORD_SIZE( sp )
	store_x x14, 10 * portWORD_SIZE( sp )
	store_x x15, 11 * portWORD_SIZE( sp )
	store_x x16, 12 * portWORD_SIZE( sp )
	store_x x17, 13 * portWORD_SIZE( sp )
	store_x x18, 14 * portWORD_SIZE( sp )
	store_x x19, 15 * portWORD_SIZE( sp )
	store_x x20, 16 * portWORD_SIZE( sp )
	store_x x21, 17 * portWORD_SIZE( sp )
	store_x x22, 18 * portWORD_SIZE( sp )
	store_x x23, 19 * portWORD_SIZE( sp )
	store_x x24, 20 * portWORD_SIZE( sp )
	store_x x25, 21 * portWORD_SIZE( sp )
	store_x x26, 22 * portWORD_SIZE( sp )
	store_x x27, 23 * portWORD_SIZE( sp )
	store_x x28, 24 * portWORD_SIZE( sp )
	store_x x29, 25 * portWORD_SIZE( sp )
	store_x x30, 26 * portWORD_SIZE( sp )
	store_x x31, 27 * portWORD_SIZE( sp )

	

	portasmSAVE_ADDITIONAL_REGISTERS	/* Defined in freertos_risc_v_chip_specific_extensions.h to save any registers unique to the RISC-V implementation. */

	addi sp, sp, -portHEADER_SIZE
	csrr t0, mstatus					/* Required for MPIE bit. */
	store_x t0, 1 * portWORD_SIZE( sp )

	load_x  t0, pxCurrentTCB			/* Load pxCurrentTCB. */
	store_x  sp, 0( t0 )				/* Write sp to first TCB member. */

	csrr a1, mepc
	store_x a1, 0( sp )					/* Save updated exception return address. */

	addi a1, x0, 0x20
	csrs 0x804, a1

	load_x sp, xISRStackTop				/* Switch to ISR stack before function call. */
	jal vTaskSwitchContext

processed_source:
	load_x  t1, pxCurrentTCB			/* Load pxCurrentTCB. */
	load_x  sp, 0( t1 )				 	/* Read sp from first TCB member. */

	/* Load mret with the address of the next instruction in the task to run next. */
	load_x t0, 0( sp )
	csrw mepc, t0

	load_x  t0, 1* portWORD_SIZE( sp )
	csrw mstatus, t0						/* Required for MPIE bit. */

	addi sp, sp, portHEADER_SIZE

	portasmRESTORE_ADDITIONAL_REGISTERS	/* Defined in freertos_risc_v_chip_specific_extensions.h to restore any registers unique to the RISC-V implementation. */	

	load_x  x1, 0 * portWORD_SIZE( sp )
	load_x  x5, 1 * portWORD_SIZE( sp )		/* t0 */
	load_x  x6, 2 * portWORD_SIZE( sp )		/* t1 */
	load_x  x7, 3 * portWORD_SIZE( sp )		/* t2 */
	load_x  x8, 4 * portWORD_SIZE( sp )		/* s0/fp */
	load_x  x9, 5 * portWORD_SIZE( sp )		/* s1 */
	load_x  x10, 6 * portWORD_SIZE( sp )	/* a0 */
	load_x  x11, 7 * portWORD_SIZE( sp )	/* a1 */
	load_x  x12, 8 * portWORD_SIZE( sp )	/* a2 */
	load_x  x13, 9 * portWORD_SIZE( sp )	/* a3 */
	load_x  x14, 10 * portWORD_SIZE( sp )	/* a4 */
	load_x  x15, 11 * portWORD_SIZE( sp )	/* a5 */
	load_x  x16, 12 * portWORD_SIZE( sp )	/* a6 */
	load_x  x17, 13 * portWORD_SIZE( sp )	/* a7 */
	load_x  x18, 14 * portWORD_SIZE( sp )	/* s2 */
	load_x  x19, 15 * portWORD_SIZE( sp )	/* s3 */
	load_x  x20, 16 * portWORD_SIZE( sp )	/* s4 */
	load_x  x21, 17 * portWORD_SIZE( sp )	/* s5 */
	load_x  x22, 18 * portWORD_SIZE( sp )	/* s6 */
	load_x  x23, 19 * portWORD_SIZE( sp )	/* s7 */
	load_x  x24, 20 * portWORD_SIZE( sp )	/* s8 */
	load_x  x25, 21 * portWORD_SIZE( sp )	/* s9 */
	load_x  x26, 22 * portWORD_SIZE( sp )	/* s10 */
	load_x  x27, 23 * portWORD_SIZE( sp )	/* s11 */
	load_x  x28, 24 * portWORD_SIZE( sp )	/* t3 */
	load_x  x29, 25 * portWORD_SIZE( sp )	/* t4 */
	load_x  x30, 26 * portWORD_SIZE( sp )	/* t5 */
	load_x  x31, 27 * portWORD_SIZE( sp )	/* t6 */
	addi sp, sp, portCONTEXT_SIZE

	mret
	
/*-----------------------------------------------------------*/

.align 8

xPortStartFirstTask:

/* if it is an assembly entry code, the SP offset value is determined by the assembly code,
but the C code is determined by the compiler, so we subtract 512 here as a reservation.
When entering the interrupt function of C code, the compiler automatically presses the stack
into the task stack. We can only change the SP value used by the calling function after switching
the interrupt stack.This problem can be solved by modifying the interrupt to the assembly entry,
and there is no need to reserve 512 bytes. You only need to switch the interrupt stack at the
beginning of the interrupt function */
	lw t0, xISRStackTop
	addi t0, t0, -512
    csrw mscratch,t0

	load_x  sp, pxCurrentTCB			/* Load pxCurrentTCB. */
	load_x  sp, 0( sp )				 	/* Read sp from first TCB member. */

	load_x  x1, 0( sp ) /* Note for starting the scheduler the exception return address is used as the function return address. */
	load_x  x5, 1 * portWORD_SIZE( sp )	/* Initial mstatus into x5 (t0) */
	addi x5, x5, 0x08						/* Set MIE bit so the first task starts with interrupts enabled - required as returns with ret not eret. */
	csrrw  x0, mstatus, x5					/* Interrupts enabled from here! */

	addi sp, sp, portHEADER_SIZE

	portasmRESTORE_ADDITIONAL_REGISTERS	/* Defined in freertos_risc_v_chip_specific_extensions.h to restore any registers unique to the RISC-V implementation. */

	load_x  x6, 2 * portWORD_SIZE( sp )		/* t1 */
	load_x  x7, 3 * portWORD_SIZE( sp )		/* t2 */
	load_x  x8, 4 * portWORD_SIZE( sp )		/* s0/fp */
	load_x  x9, 5 * portWORD_SIZE( sp )		/* s1 */
	load_x  x10, 6 * portWORD_SIZE( sp )	/* a0 */
	load_x  x11, 7 * portWORD_SIZE( sp )	/* a1 */
	load_x  x12, 8 * portWORD_SIZE( sp )	/* a2 */
	load_x  x13, 9 * portWORD_SIZE( sp )	/* a3 */
	load_x  x14, 10 * portWORD_SIZE( sp )	/* a4 */
	load_x  x15, 11 * portWORD_SIZE( sp )	/* a5 */
	load_x  x16, 12 * portWORD_SIZE( sp )	/* a6 */
	load_x  x17, 13 * portWORD_SIZE( sp )	/* a7 */
	load_x  x18, 14 * portWORD_SIZE( sp )	/* s2 */
	load_x  x19, 15 * portWORD_SIZE( sp )	/* s3 */
	load_x  x20, 16 * portWORD_SIZE( sp )	/* s4 */
	load_x  x21, 17 * portWORD_SIZE( sp )	/* s5 */
	load_x  x22, 18 * portWORD_SIZE( sp )	/* s6 */
	load_x  x23, 19 * portWORD_SIZE( sp )	/* s7 */
	load_x  x24, 20 * portWORD_SIZE( sp )	/* s8 */
	load_x  x25, 21 * portWORD_SIZE( sp )	/* s9 */
	load_x  x26, 22 * portWORD_SIZE( sp )	/* s10 */
	load_x  x27, 23 * portWORD_SIZE( sp )	/* s11 */
	load_x  x28, 24 * portWORD_SIZE( sp )	/* t3 */
	load_x  x29, 25 * portWORD_SIZE( sp )	/* t4 */
	load_x  x30, 26 * portWORD_SIZE( sp )	/* t5 */
	load_x  x31, 27 * portWORD_SIZE( sp )	/* t6 */

	
	load_x  x5, 1 * portWORD_SIZE( sp )		/* Initial x5 (t0) value. */

	addi	sp, sp, portCONTEXT_SIZE
	ret
	
/*-----------------------------------------------------------*/

/*
 * Unlike other ports pxPortInitialiseStack() is written in assembly code as it
 * needs access to the portasmADDITIONAL_CONTEXT_SIZE constant.  The prototype
 * for the function is as per the other ports:
 * StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );
 *
 * As per the standard RISC-V ABI pxTopcOfStack is passed in in a0, pxCode in
 * a1, and pvParameters in a2.  The new top of stack is passed out in a0.
 *
 */
.align 8

pxPortInitialiseStack:
	csrr t0, mstatus					/* Obtain current mstatus value. */
	andi t0, t0, ~0x8					/* Ensure interrupts are disabled when the stack is restored within an ISR.  Required when a task is created after the schedulre has been started, otherwise interrupts would be disabled anyway. */
	addi t1, x0, 0x40
	slli t1, t1, 8
	not  t1, t1
	and  t0, t0, t1					/* Clear FS dirty bit */	
	addi t1, x0, 0x388					/* Generate the value 00x3880, which are the MPIE, MPP and (FS = Initial) bits to set in mstatus. */
	slli t1, t1, 4
	or t0, t0, t1						/* Set MPIE and MPP bits in mstatus value. */

	/* MEPC / MSTATUS */
	/* FIXME : since it is the initial step, we dont need to allocated portasmADDITIONAL_CONTEXT_SIZE */
	addi a0, a0, -(portasmADDITIONAL_CONTEXT_SIZE+portCONTEXT_SIZE+portHEADER_SIZE)
	store_x a1, 0 * portWORD_SIZE(a0)				/* mret value (pxCode parameter) onto the stack. */
	store_x t0, 1 * portWORD_SIZE(a0)				/* Mstatus */
	addi    a1, a0 ,0
	addi a1, a1, portHEADER_SIZE 					/* Skip header */
	addi a1, a1, portasmADDITIONAL_CONTEXT_SIZE  	/* PLACEHOLDER FOR FPU */		
	store_x a2, 6*portWORD_SIZE(a1)	 				/* param-> x10 */
	ret
	


