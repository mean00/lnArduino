/**
 * "simple" api to PIO
 *
 *
 *
 */

#include "ln_rp_pio.h"
#include "lnArduino.h"
#include "ln_rp_clocks.h"
#include "ln_rp_pio_priv.h"

static int consumedPioSpace[2] = {0, 0};
static const LN_RP_PIO *rpiox[2] = {(LN_RP_PIO *)LN_PIO0_BASE_ADR, (LN_RP_PIO *)LN_PIO1_BASE_ADR};
#define ENGINE() ((LN_RP_PIO *)rpiox[_unit])

/**
 *
 */
static void startPIO(LN_RP_PIO *rpio, uint32_t sm)
{
    uint32_t ctrl = rpio->PIO_CTRL;
    ctrl |= LN_RP_PIO_CTRL_CLKDIV_RESTART_BIT(sm); // restart clock all SM
    ctrl |= LN_RP_PIO_CTRL_RESTART_BIT(sm);        // restart SM
    ctrl |= LN_RP_PIO_CTRL_ENABLE_BIT(sm);         // and go...
    rpio->PIO_CTRL = ctrl;
}
/**
 *
 */
static void stopPIO(LN_RP_PIO *rpio, uint32_t sm)
{
    uint32_t ctrl = rpio->PIO_CTRL;
    ctrl &= ~LN_RP_PIO_CTRL_ENABLE_BIT(sm); // disable all SM
    rpio->PIO_CTRL = ctrl;
}
/**
 *
 */
static void resetPIO(LN_RP_PIO *rpio, uint32_t sm)
{
    uint32_t ctrl = rpio->PIO_CTRL;
    ctrl &= ~LN_RP_PIO_CTRL_ENABLE_BIT(sm);        // disable all SM
    ctrl |= LN_RP_PIO_CTRL_RESTART_BIT(sm);        // disable all SM
    ctrl |= LN_RP_PIO_CTRL_CLKDIV_RESTART_BIT(sm); // disable all SM
    rpio->PIO_CTRL = ctrl;
}
/**
 *
 */
rpPIO::rpPIO(uint32_t unit)
{
    _unit = unit;
    xAssert(unit == 1 || unit == 0);
    resetPIO(ENGINE(), 0);
}
/**
 *
 */
rpPIO::~rpPIO()
{
    resetPIO(ENGINE(), 0);
    consumedPioSpace[_unit] = 0;
}
/**
 *
 *
 */
rpPIO_SM *rpPIO::getSm(uint32_t sm)
{
    return new rpPIO_SM(_unit, sm);
}
//===============================================================================
//===============================================================================
//===============================================================================
//===============================================================================
rpPIO_SM::rpPIO_SM(uint32_t unit, uint32_t sm)
{
    _unit = unit;
    _sm = sm;
}
/**
 *
 */
bool rpPIO_SM::uploadCode(uint32_t codeSize, const uint16_t *code, uint32_t wrapBegin, uint32_t wrapEnd)
{
    _codeSize = codeSize;
    _wrapStart = wrapBegin;
    _wrapEnd = wrapEnd;
    xAssert((consumedPioSpace[_unit] + codeSize) <= LN_RP_PIO_MAX_PROGRAM_SPACE);
    _codeOffset = consumedPioSpace[_unit];
    consumedPioSpace[_unit] += codeSize;

    uint32_t *ptr = (uint32_t *)(&(ENGINE()->PIO_INSTR_MEM[_codeOffset]));
    // Upload
    for (int i = 0; i < codeSize; i++)
    {
        ptr[i] = (uint32_t)code[i]; // 16->32, instructionis 16 bits, reg is 32
    }
    // set wrap
    ENGINE()->PIO_SM[_sm].EXECCTRL =
        LN_RP_PIO_SM_EXECCTRL_WRAP_BOTTOM_BIT(wrapEnd) + LN_RP_PIO_SM_EXECCTRL_WRAP_TOP_BIT(wrapBegin);

    return true;
}
/**
 */
bool rpPIO_SM::setSpeed(uint32_t fq)
{
    uint32_t fq_in = clock_get_hz(clk_sys);
    uint32_t intdiv = fq_in / fq;
// JUST FOR TESTING
#warning FOR TEST ONLY
    ENGINE()->PIO_SM[_sm].CLKDIV = LN_RP_PIO_SM_CLKDIV(intdiv, 0);
    return false;
}
/**
 * THIS IS INCCORECT TODO **FIXME**
 */
bool rpPIO_SM::configure(const rpPIO_pinConfig &config)
{
#warning FIXME TEMPORARY
    uint32_t pin_ctrol = LN_RP_PIO_SM_PINCTRL_OUT_BASE_BIT(config.outputs.startPin) + // assume base = set
                         LN_RP_PIO_SM_PINCTRL_OUT_COUNT_BIT(config.outputs.pinNb);
    pin_ctrol |=
        LN_RP_PIO_SM_PINCTRL_SET_BASE_BIT(config.sets.startPin) + LN_RP_PIO_SM_PINCTRL_SET_COUNT_BIT(config.sets.pinNb);
    pin_ctrol |= LN_RP_PIO_SM_PINCTRL_SIDESET_BASE_BIT(config.sidesets.startPin) +
                 LN_RP_PIO_SM_PINCTRL_SIDESET_COUNT_BIT(config.sidesets.pinNb);
    pin_ctrol |= LN_RP_PIO_SM_PINCTRL_IN_BASE_BIT(config.inputs.startPin);
    ENGINE()->PIO_SM[_sm].PINCTRL = pin_ctrol;
    ENGINE()->PIO_SM[_sm].SHIFTCTRL =
        LN_RP_PIO_SM_SHIFTCTRL_OUT_SHIFTDIR + // shift output to the right, input to the left
        0;
    return true;
}
/**
 *
 */
bool rpPIO_SM::execute()
{
    ENGINE()->PIO_SM[_sm].INSTR = _codeOffset; // set PC
    startPIO(ENGINE(), _sm);
    return true;
}
/**
 *
 */
bool rpPIO_SM::stop()
{
    stopPIO(ENGINE(), _sm);
    return true;
}
/**
 *
 *
 */
bool rpPIO_SM::read(uint32_t nb, uint32_t *data)
{
    xAssert(0);
}
bool rpPIO_SM::write(uint32_t nb, uint32_t *data)
{
#warning INCOMPLETE/ TEMPORARY
    ENGINE()->PIO_TXF[0] = data[0];
    return true;
}
// -- EOF
