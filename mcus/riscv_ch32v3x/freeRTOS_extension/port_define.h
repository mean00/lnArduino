/*
 *
 */
#pragma once
#if __riscv_xlen == 64
	#define portWORD_SIZE 8
	#define store_x sd
	#define load_x ld
	#error "64 bits not supported"
#elif __riscv_xlen == 32
	#define store_x sw
	#define load_x lw
	#define portWORD_SIZE 4
	#define portCONTEXT_COUNT 28
	#define portHEADER_COUNT  2
	#if ARCH_FPU==1
		#define portFPU_COUNT 32
	#else
		#define portFPU_COUNT 0
	#endif

#else
	#error Assembler did not define __riscv_xlen
#endif


/* Only the standard core registers are stored by default.  Any additional
registers must be saved by the portasmSAVE_ADDITIONAL_REGISTERS and
portasmRESTORE_ADDITIONAL_REGISTERS macros - which can be defined in a chip
specific version of freertos_risc_v_chip_specific_extensions.h.  See the notes
at the top of this file. */
#define portCONTEXT_SIZE ( portCONTEXT_COUNT * portWORD_SIZE )
#define portHEADER_SIZE ( portHEADER_COUNT * portWORD_SIZE )
#define portFPU_SIZE  (portFPU_COUNT * portWORD_SIZE)

