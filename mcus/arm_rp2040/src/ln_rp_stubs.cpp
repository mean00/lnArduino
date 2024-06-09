

extern "C" void mutex_init(void *mtx)
{
}
extern "C" void recursive_mutex_init(void *mtx)
{
}
extern "C" void spin_locks_reset(void)
{
}

extern "C" void watchdog_start_tick(unsigned int cycles)
{
}

extern "C" bool running_on_fpga()
{
    return false;
}

extern "C" void hw_claim_lock()
{
}
extern "C" void hw_claim_unlock()
{
}
#include "lnArduino.h"
extern "C"
{
#include "hardware/gpio.h"
    void gpio_set_function(unsigned int pin, enum gpio_function pio)
    {
        switch (pio)
        {
        case GPIO_FUNC_PIO0:
            lnPinMode((lnPin)pin, lnRP_PIO0_MODE);
            return;
            break;
        case GPIO_FUNC_PIO1:
            lnPinMode((lnPin)pin, lnRP_PIO1_MODE);
            return;
            break;
        default:
            xAssert(0);
            break;
        }
    }
    void *rom_func_lookup(uint32_t code)
    {
        return NULL;
    }
}
