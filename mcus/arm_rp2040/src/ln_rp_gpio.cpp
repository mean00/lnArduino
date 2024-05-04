#include "ln_rp_gpio.h"
#include "lnArduino.h"
#include "lnGPIO.h"

/**
    \fn
*/
void lnPinMode(const lnPin pin, const lnGpioMode mode, const int speedInMhz)
{
    uint32_t fun = LN_RP_GPIO_CONTROL_FUNC(SIO);
    uint32_t pad, control;
    switch (mode)
    {
    case lnSPI_MODE:
        fun = LN_RP_GPIO_CONTROL_FUNC(SPI);
        lnGpio->PINS[pin].control = fun + LN_RP_GPIO_CONTROL_OE(NORMAL);
        return;
    case lnUART:
        fun = LN_RP_GPIO_CONTROL_FUNC(UART);
        lnGpio->PINS[pin].control = fun + LN_RP_GPIO_CONTROL_OE(NORMAL);
        return;
    case lnFLOATING:
        //
        pad = LN_RP_PADS_OUTPUT_DISABLE; // input disabled by default
        control = LN_RP_GPIO_CONTROL_OE_DISABLE;
        break;
    case lnINPUT_FLOATING:
        pad = LN_RP_PADS_INPUT_ENABLE + LN_RP_PADS_OUTPUT_DISABLE; //
        control = LN_RP_GPIO_CONTROL_OE_DISABLE;
        break;
    case lnINPUT_PULLUP:
        pad = LN_RP_PADS_INPUT_ENABLE + LN_RP_PADS_OUTPUT_DISABLE + LN_RP_PADS_PULLUP; //
        control = LN_RP_GPIO_CONTROL_OE_DISABLE;
        break;
    case lnINPUT_PULLDOWN:
        pad = LN_RP_PADS_INPUT_ENABLE + LN_RP_PADS_OUTPUT_DISABLE + LN_RP_PADS_PULLDOWN; //
        control = LN_RP_GPIO_CONTROL_OE_DISABLE;
        break;
    case lnOUTPUT_OPEN_DRAIN:
        pad = LN_RP_PADS_OUTPUT_DISABLE;
        control = LN_RP_GPIO_CONTROL_OE_DISABLE;
        break;
    case lnOUTPUT:
        pad = LN_RP_PADS_DRIVE(12MA);
        control = LN_RP_GPIO_CONTROL_OE(ENABLE); // 12 mA
        break;
    default:
        xAssert(0);
        break;
    }
    lnPads->PADS[pin] = pad | LN_RP_PADS_SLEW_FAST;
    lnGpio->PINS[pin].control = fun + control;
    /*
    lnOUTPUT_OPEN_DRAIN = 4,
    lnALTERNATE_PP,
    lnALTERNATE_OD,
    lnPWM,
    lnADC_MODE,
    lnDAC_MODE
    */
}
/**
 * @brief
 *
 */
void lnOpenDrainClose(const lnPin pin, const bool close)
{
    uint32_t fun = LN_RP_GPIO_CONTROL_FUNC(SIO);
    uint32_t pad, control;

    if (close) // passing
    {
        lnSio->GPIO_OUT_CLR = 1 << pin;
        pad = LN_RP_PADS_DRIVE(12MA);
        control = LN_RP_GPIO_CONTROL_OE(ENABLE); // 12 mA
    }
    else // hi z
    {
        pad = LN_RP_PADS_OUTPUT_DISABLE;
        control = LN_RP_GPIO_CONTROL_OE_DISABLE;
    }
    lnPads->PADS[pin] = pad;
    lnGpio->PINS[pin].control = fun + control;
}
/**
    \fn
*/

void lnDigitalWrite(const lnPin pin, bool value)
{
    if (value)
        lnSio->GPIO_OUT_SET = 1 << pin;
    else
        lnSio->GPIO_OUT_CLR = 1 << pin;
}
/**
    \fn
*/

bool lnDigitalRead(const lnPin pin)
{
    return !!(lnSio->GPIO_IN & (1 << pin));
}
/**
    \fn
*/

void lnDigitalToggle(const lnPin pin)
{
    lnSio->GPIO_OUT_XOR = 1 << pin;
}
/**
    \fn
    //----------------- This is bluepill common specific !-------------
*/

volatile uint32_t *lnGetGpioOnRegister(int port) // Bop register for port "port" with port A:0, B:1, ...
{
    return &(lnSio->GPIO_OUT_SET);
}
volatile uint32_t *lnGetGpioOffRegister(int port) // Bop register for port "port" with port A:0, B:1, ...
{
    return &(lnSio->GPIO_OUT_CLR);
}
/**
    \fn
     Direction register for the bit 0..7 of port "port" , A=0, B=1, ...
     //----------------- This is bluepill common specific !-------------
*/

volatile uint32_t *lnGetGpioDirectionRegister(int port)
{
    xAssert(0);
    return NULL;
}
/**
    \fn
    //----------------- This is bluepill common specific !-------------
*/

volatile uint32_t *lnGetGpioValueRegister(int port) // Bit value for LOW bits of port "port"
{
    xAssert(0);
    return NULL;
}
/**
    \fn
    //----------------- This is bluepill common specific !-------------
*/

uint32_t lnReadPort(int port)
{
    xAssert(0);
    return 0;
}
/**
 * @brief Construct a new ln Fast I O::ln Fast I O object
 *
 */
lnFastIO::lnFastIO(lnPin pin)
{
    lnPinMode(pin, lnOUTPUT);
    _on = &(lnSio->GPIO_OUT_SET);
    _off = &(lnSio->GPIO_OUT_CLR);
    _bit = 1 << pin;
}
// EOF
