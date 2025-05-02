/*
 *  (C) 2022/2023 MEAN00 fixounet@free.fr
 *  See license file
 *  PFIC is configured with 2 nested level, 1 bit for preemption
 *  That means interrupt priority between 0..7 ignoring preemption
 *
 *
 */

#ifdef USE_CH32v3x_HW_IRQ_STACK
#define HANDLER_DESC(x)                                                                                                \
    extern "C" void x();                                                                                               \
    extern "C" void x##_relay() LN_INTERRUPT_TYPE;
#define HANDLER_DESC_C(y)                                                                                              \
    extern "C" void y();                                                                                               \
    extern "C" void y##_relay() LN_INTERRUPT_TYPE;
extern "C" void unsupported_relay();
#define LOCAL_LN_INTERRUPT_TYPE
#define WCH_HW_STACK CH32_SYSCR_HWSTKEN
#else
#define LOCAL_LN_INTERRUPT_TYPE LN_INTERRUPT_TYPE
#define HANDLER_DESC(x) extern "C" void x() LOCAL_LN_INTERRUPT_TYPE;
#define HANDLER_DESC_C(y) extern "C" void y() LOCAL_LN_INTERRUPT_TYPE;
#define WCH_HW_STACK 0
#endif

#include "ch32v30x_isr_helper.h"
#include "ch32v3x_interrupt_table.h"
#include "lnArduino.h"
#include "lnIRQ.h"
#include "lnIRQ_riscv_priv_ch32v3x.h"
#include "lnRCU.h"

extern "C"
{

    extern const char _data_begin, _data_end;
    extern const char _bss_begin, _bss_end;
    extern const char _data_lma;

    int main();
    void __libc_init_array(void);

    /**
     * @brief
     *
     */
    ISR_CODE void __attribute__((noreturn)) start_c()
    {

        __asm volatile("  mv t0, %0 \n" // src
                       "  mv t1, %1 \n" // dst
                       "  mv t2, %2 \n" // end
                       "lp0: \n"
                       "  lw t3, 0(t0) \n"
                       "  sw t3, 0(t1) \n"
                       "  addi t0,t0,4 \n"
                       "  addi t1,t1,4 \n"
                       "  bgt  t2,t1,lp0 \n"

                       "  mv t0, %3 \n" // begin
                       "  mv t1, %4 \n" // end
                       "lp1: \n"
                       "  sw x0, 0(t0) \n"
                       "  addi t0,t0,4 \n"
                       "  bgt  t1,t0,lp1 \n"

                       ::"r"((uint32_t *)&_data_lma), // 0 src
                       "r"((uint32_t *)&_data_begin), // 1 data
                       "r"((uint32_t *)&_data_end),   // 2 end
                       "r"((uint32_t *)&_bss_begin),  // 3 zstart
                       "r"((uint32_t *)&_bss_end));   // 4 zend

        __libc_init_array(); // call ctor before jumping in the code
        main();
        xAssert(0);
    }
}

// EOF
