/* Original license :
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * Modified by mean00
 */

.word data_cpy_table // we may need to decode pointers that are in RAM at runtime.


.section .vectors, "ax"
.align 2

.global __vectors, __VECTOR_TABLE
__VECTOR_TABLE:
__vectors:
.word __StackTop
.word _reset_handler  // minimalistic vector table


// ELF entry point:
.type _entry_point,%function
.thumb_func
.global _entry_point
_entry_point:
Reset_Handler:
_reset_handler:

    // put a proper stack in case we loaded through gdb 
    ldr r0, =__vectors 
    ldr r0, [r0]
    msr msp, r0
    # if we are core #1 , go loop somehwere and let core 0 go
    ldr r0, =0xD0000000UL// (SIO_BASE + SIO_CPUID_OFFSET)
    ldr r0, [r0]
    cmp r0, #0
    bne hold_non_core0_in_bootrom
    // copy data_cpy
    adr r4, data_cpy_table

    // assume there is at least one entry
1:
    ldmia r4!, {r1-r3}
    cmp r1, #0
    beq 2f
    bl data_cpy
    b 1b
2:

 
    // zero bss
bss_header_start:
    // Zero out the BSS
    //--------------------------
    // Header to align to 64
    //--------------------------
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__
    movs r0, #0

    // Header to align to 64
    ldr r4, = 63 // 64 round down
3:
    mov r5, r4 // 64 round down
    ands r5, r1
    beq bss_header_done
    stmia r1!,{r0} 
    b 3b
    //--------------------------
    // No r1 is 64 aligned
    // Body 64 bytes at a time 
    //--------------------------

bss_header_done:
    ldr r0, = 0xffffffc0 // 64 round down
    ands r2, r0          // round down end address
    movs r3, #0
    movs r4, #0
    movs r5, #0
    movs r6, #0
    b bss_fill_test
bss_fill_loop:                // 4x16 bytes = 64 bytes at a time
    stmia r1!, {r3,r4,r5,r6}  // 16 bytes at a time
    stmia r1!, {r3,r4,r5,r6}  // 16 bytes at a time
    stmia r1!, {r3,r4,r5,r6}  // 16 bytes at a time
    stmia r1!, {r3,r4,r5,r6}  // 16 bytes at a time
bss_fill_test:
    cmp r1, r2
    bne bss_fill_loop
    //--------------------------
    // Trailer to align to 64
    //--------------------------

    ldr r2, =__bss_end__
    b bss_trailer_end
    
bss_trailer: 
    stmia r1!,{r3}
bss_trailer_end:
    cmp r1, r2
    bne bss_trailer


platform_entry: // symbol for stack traces
    ldr r1, =pre_main // jump to main
    blx r1
    bkpt #0
#
#
#
hold_non_core0_in_bootrom:
    nop 
    b hold_non_core0_in_bootrom


data_cpy_loop:
    ldm r1!, {r0}
    stm r2!, {r0}
data_cpy:
    cmp r2, r3
    blo data_cpy_loop
    mov pc,lr

.align 2
data_cpy_table:

.word __ln_copy_ram_text_source__
.word __ln_copy_ram_text_start__
.word __ln_copy_ram_text_end__

.word __etext // Might be a duplicate
.word __data_start__
.word __data_end__

.word __scratch_x_source__
.word __scratch_x_start__
.word __scratch_x_end__

.word __scratch_y_source__
.word __scratch_y_start__
.word __scratch_y_end__


.word 0 // null terminator
// EOF
//
