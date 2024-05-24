
#include "hardware/irq.h"
#include "hardware/regs/intctrl.h"
#include "lnArduino.h"
#include "stdio.h"

#include "hardware/structs/scb.h"
volatile armv6m_scb_t *scb = ((volatile armv6m_scb_t *)(PPB_BASE + M0PLUS_CPUID_OFFSET));

/*
0 msp
1 reset

    SVC => 11
    PEND => 14
    SYSTICK => 15
*/

extern uint32_t ram_vector_table[48]; // this is the vector table in RAM

void lnSetInterruptHandler(const LnIRQ &irq, lnInterruptHandler *handler)
{
    int real_irq = (int)irq + 16;
    ram_vector_table[real_irq] = (uint32_t)handler;
    __asm__("dmb" : : : "memory");
}
/**
 * @brief
 *
 * @param irq
 */
void lnEnableInterrupt(const LnIRQ &irq)
{
    irq_set_enabled(irq, true);
}
/**
 * @brief
 *
 * @param irq
 */
void lnDisableInterrupt(const LnIRQ &irq)
{
    irq_set_enabled(irq, false);
}
/**
 * @brief
 *
 */
extern "C" void dummy_caller(int code)
{
    volatile uint32_t vtor = scb->vtor;
    __asm__("bkpt #0");
}
#if 1
extern "C" void irq_init_priorities() {
}
/**
 *
 */
void irq_set_mask_enabled(uint32_t mask, bool enabled) {
    if (enabled) {
        // Clear pending before enable
        // (if IRQ is actually asserted, it will immediately re-pend)
        *((io_rw_32 *) (PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)) = mask;
        *((io_rw_32 *) (PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)) = mask;
    } else {
        *((io_rw_32 *) (PPB_BASE + M0PLUS_NVIC_ICER_OFFSET)) = mask;
    }
}
/**
 *
 */
extern "C" void irq_set_enabled(uint num, bool enabled) {
    irq_set_mask_enabled(1u << num, enabled);
}
/**
*
*/
extern "C" void irq_set_exclusive_handler(uint num, irq_handler_t handler) {
    lnSetInterruptHandler((LnIRQ )num,  handler);
}

#endif
