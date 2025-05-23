
#include "driver/gpio.h"
#include "lnArduino.h"
#include "lnGPIO_pins.h"
extern "C"
{
#include "soc/gpio_periph.h"
// #include "/register/soc/gpio_struct.h
#include "register/soc/gpio_struct.h"
}
/**
 */
void lnPinMode(const lnPin pin, const lnGpioMode mode, const int speedInMhz)
{
    gpio_config_t io_conf = {.pin_bit_mask = (1ULL << pin),
                             .mode = GPIO_MODE_OUTPUT,
                             .pull_up_en = GPIO_PULLUP_DISABLE,
                             .pull_down_en = GPIO_PULLDOWN_DISABLE,
                             .intr_type = GPIO_INTR_DISABLE};

    switch (mode)
    {
    case lnFLOATING:
        io_conf.mode = GPIO_MODE_DISABLE;
    case lnINPUT_FLOATING:
        xAssert(0);
        break;
    case lnINPUT_PULLUP:
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        xAssert(0);
        break;
    case lnINPUT_PULLDOWN:
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
        xAssert(0);
        break;
    case lnOUTPUT:
        break;
    case lnOUTPUT_OPEN_DRAIN:
        xAssert(0);
        break;
    case lnALTERNATE_PP:
        xAssert(0);
        break;
    case lnALTERNATE_OD:
        xAssert(0);
        break;
    case lnPWM:
        xAssert(0);
        break;
    case lnADC_MODE:
        xAssert(0);
        break;
    case lnDAC_MODE:
        xAssert(0);
        break;
    case lnUART:
        xAssert(0);
        break;
    case lnSPI_MODE:
        xAssert(0);
        break;
    case lnUART_Alt:
        xAssert(0);
        break;
    default:
        xAssert(0);
        break;
    }
    gpio_config(&io_conf);
}
/**
 *
 *
 */
void lnDigitalWrite(const lnPin pin, bool value)
{
    gpio_set_level((gpio_num_t)pin, value);
}
/**
 *
 *
 */
bool lnDigitalRead(const lnPin pin)
{
    return gpio_get_level((gpio_num_t)pin);
}
/**
 *
 *
 */
void lnDigitalToggle(const lnPin pin)
{
    gpio_set_level((gpio_num_t)pin, !gpio_get_level((gpio_num_t)pin));
}
/**
 *
 */
void lnOpenDrainClose(const lnPin pin, const bool close) // if true, the open drain is passing, else it is hiz
{
    xAssert(0);
}
/**
 *
 */
lnFastIO::lnFastIO(lnPin p)
{
    lnPinMode(p, lnOUTPUT);
    gpio_dev_t *hw = &GPIO;
    _bit = 1 << (uint32_t)p;
    _on = &(hw->out_w1ts.val);
    _off = &(hw->out_w1tc.val);
}

//  EOF
