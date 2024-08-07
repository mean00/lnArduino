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
    // Purge RX
    const uint32_t msk = LN_RP_PIO_FSTAT_RX_EMPTY_BIT(sm);
    while(1)
    {
        uint32_t fstat = rpio->PIO_FSTAT;
        if (fstat & msk)
            return;
        volatile uint32_t x=rpio->PIO_RXF[sm];
    }
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
 * when wrapEnd is reached, it jumps tp wrapBegin
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
    // when wrapEnd is reached, it jumps tp wrapBegin
    // when TOP is reached it jumps to BOTTOM
    ENGINE()->PIO_SM[_sm].EXECCTRL =
        LN_RP_PIO_SM_EXECCTRL_WRAP_TOP_BIT_SET(wrapEnd) + LN_RP_PIO_SM_EXECCTRL_WRAP_BOTTOM_BIT_SET(wrapBegin);

    return true;
}
/**

 */
bool rpPIO_SM::setBitOrder(bool inputMsbFirst, bool outputMsbFirst)
{
    uint32_t shiftctrl = ENGINE()->PIO_SM[_sm].SHIFTCTRL;
    if (outputMsbFirst)
    {
        shiftctrl &= ~LN_RP_PIO_SM_SHIFTCTRL_OUT_SHIFTDIR; // shift output to the left, input to the left
    }
    else
    {
        shiftctrl |= LN_RP_PIO_SM_SHIFTCTRL_OUT_SHIFTDIR; // shift output to the right, input to the left
    }
    if (!inputMsbFirst)
    {
        shiftctrl &= ~LN_RP_PIO_SM_SHIFTCTRL_IN_SHIFTDIR; // shift output to the left, input to the left
    }
    else
    {
        shiftctrl |= LN_RP_PIO_SM_SHIFTCTRL_IN_SHIFTDIR; // shift output to the right, input to the left
    }
    ENGINE()->PIO_SM[_sm].SHIFTCTRL = shiftctrl;
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
    // now program the thingie
    uint32_t pin_ctrol = 0;
    pin_ctrol |= LN_RP_PIO_SM_PINCTRL_OUT_BASE_BIT(config.outputs.startPin) +
                 LN_RP_PIO_SM_PINCTRL_OUT_COUNT_BIT(config.outputs.pinNb);
    pin_ctrol |= LN_RP_PIO_SM_PINCTRL_SET_BASE_BIT(config.sets.startPin) + //
                 LN_RP_PIO_SM_PINCTRL_SET_COUNT_BIT(config.sets.pinNb);
    pin_ctrol |= LN_RP_PIO_SM_PINCTRL_IN_BASE_BIT(config.inputs.startPin);
    // Use 1 state pine

    ENGINE()->PIO_SM[_sm].PINCTRL = pin_ctrol;
    return true;
}
/**
 *
 */
#define PIO_INSTR_SET 7
#define SET_DEST_PINDIR 4
bool rpPIO_SM::setPinDir(lnPin pin, bool isOutput)
{
    uint32_t oldshift = ENGINE()->PIO_SM[_sm].SHIFTCTRL;
    uint32_t oldpin = ENGINE()->PIO_SM[_sm].PINCTRL;

    ENGINE()->PIO_SM[_sm].SHIFTCTRL = 0;
    uint32_t control = LN_RP_PIO_SM_PINCTRL_SET_BASE_BIT(pin) + LN_RP_PIO_SM_PINCTRL_SET_COUNT_BIT(1);
    int instruction = (PIO_INSTR_SET << 13) + (SET_DEST_PINDIR << 5) + (isOutput << 0);
    ENGINE()->PIO_SM[_sm].PINCTRL = control;
    ENGINE()->PIO_SM[_sm].INSTR = instruction;

    ENGINE()->PIO_SM[_sm].SHIFTCTRL = oldshift;
    ENGINE()->PIO_SM[_sm].PINCTRL = oldpin;
    return true;
}
/**
 *
 *
 */
bool rpPIO_SM::waitTxEmpty()
{
    const uint32_t msk = LN_RP_PIO_FSTAT_TX_EMPTY_BIT(_sm);
    while (1)
    {
        uint32_t fstat = ENGINE()->PIO_FSTAT;
        if (fstat & msk)
            return true;
    }
}
/**
 *
 *
 */
bool rpPIO_SM::waitTxReady()
{
    const uint32_t msk = LN_RP_PIO_FSTAT_TX_FULL_BIT(_sm);
    while (1)
    {
        uint32_t fstat = ENGINE()->PIO_FSTAT;
        if (!(fstat & msk))
            return true;
    }
}
/**
 *
 *
 */
bool rpPIO_SM::waitRxReady()
{
    const uint32_t msk = LN_RP_PIO_FSTAT_RX_EMPTY_BIT(_sm);
    while (1)
    {
        uint32_t fstat = ENGINE()->PIO_FSTAT;
        if (!(fstat & msk))
            return true;
    }
}

/**
 *
 */
bool rpPIO_SM::configureSideSet(int startPin, int nbPin, int sideNbBits, bool optional)
{
    uint32_t pin_ctrol = ENGINE()->PIO_SM[_sm].PINCTRL;
    pin_ctrol &= ~LN_RP_PIO_SM_PINCTRL_SIDESET_COUNT_BIT(7); // this should be log2(actual set number)
    pin_ctrol &= ~LN_RP_PIO_SM_PINCTRL_SIDESET_BASE_BIT(0x1F);
    pin_ctrol |= LN_RP_PIO_SM_PINCTRL_SIDESET_COUNT_BIT(sideNbBits); // this should be log2(actual set number)
    pin_ctrol |= LN_RP_PIO_SM_PINCTRL_SIDESET_BASE_BIT(startPin);
    ENGINE()->PIO_SM[_sm].PINCTRL = pin_ctrol;

    uint32_t exec = ENGINE()->PIO_SM[_sm].EXECCTRL;
    if (nbPin && optional)
    {
        exec |= LN_RP_PIO_SM_EXECCTRL_SIDE_EN;
    }
    else
    {
        exec &= ~LN_RP_PIO_SM_EXECCTRL_SIDE_EN;
    }
    ENGINE()->PIO_SM[_sm].EXECCTRL = exec;
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
    xAssert(nb == 1);
    // wait for rx fifo not empty
#if 0
    while (1)
    {
        uint32_t level = ENGINE()->PIO_LEVEL;
        level = ((level >> (8 * _sm)) >> 4) & 0xf; // RX level
        if (level)
        {
            break;
        }
    }
#endif
    *data = ENGINE()->PIO_RXF[_sm];
    return true;
}
/**
 *
 *
 */
bool rpPIO_SM::write(uint32_t nb, uint32_t *data)
{
#warning INCOMPLETE/ TEMPORARY
    xAssert(nb == 1);
    ENGINE()->PIO_TXF[_sm] = data[0];
    return true;
}
// -- EOF
