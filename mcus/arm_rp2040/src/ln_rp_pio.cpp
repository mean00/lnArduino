/**
 * "simple" api to PIO
 *
 *
 *
 */


#include "lnArduino.h"
#include "ln_rp_pio_priv.h"
#include "ln_rp_pio.h"
#include "ln_rp_clocks.h"

static int consumedPioSpace[2]={0,0};
static const LN_RP_PIO *rpiox[2] = { (LN_RP_PIO *) LN_PIO0_BASE_ADR , (LN_RP_PIO *) LN_PIO1_BASE_ADR };
#define ENGINE() ((LN_RP_PIO *)rpiox[_unit])


/**
 *
 */
static void startPIO(LN_RP_PIO *rpio, uint32_t sm)
{
  uint32_t ctrl = rpio->PIO_CTRL;
  ctrl|=LN_RP_PIO_CTRL_CLKDIV_RESTART_BIT(sm); // restart clock all SM
  ctrl|=LN_RP_PIO_CTRL_RESTART_BIT(sm); // restart SM
  ctrl|=LN_RP_PIO_CTRL_ENABLE_BIT(sm); // and go...
  rpio->PIO_CTRL=ctrl;
}
/**
 *
 */
static void stopPIO(LN_RP_PIO *rpio, uint32_t sm)
{
  uint32_t ctrl = rpio->PIO_CTRL;
  ctrl&=~LN_RP_PIO_CTRL_ENABLE_BIT(sm); // disable all SM
  rpio->PIO_CTRL=ctrl;
}
/**
 *
 */
static void resetPIO( LN_RP_PIO *rpio,uint32_t sm )
{
  uint32_t ctrl = rpio->PIO_CTRL;
  ctrl&=~LN_RP_PIO_CTRL_ENABLE_BIT(sm); // disable all SM
  ctrl|=LN_RP_PIO_CTRL_RESTART_BIT(sm); // disable all SM
  ctrl|=LN_RP_PIO_CTRL_CLKDIV_RESTART_BIT(sm); // disable all SM
  rpio->PIO_CTRL=ctrl;
}
/**
 *
 */
rpPIO::rpPIO(uint32_t unit)
{
  _unit = unit;
  xAssert(unit==1 || unit==2);
  _codeSize=-1;
  _offset=-1;
  resetPIO(ENGINE(),0);
}
/**
  *
  */
rpPIO::~rpPIO()
{
  resetPIO(ENGINE(),0);
  consumedPioSpace[_unit]=0;
}
/**
  *
  */
bool rpPIO::uploadCode(uint32_t codeSize, const uint16_t *code)
{
  _codeSize=codeSize;
  _offset =0;
  xAssert(consumedPioSpace[_unit]==0);
  xAssert(codeSize<= LN_RP_PIO_MAX_PROGRAM_SPACE);
  uint32_t offset= consumedPioSpace[_unit];
  consumedPioSpace[_unit]=codeSize;

  uint32_t *ptr = (uint32_t *)(ENGINE()->PIO_INSTR_MEM[_offset]);
  // Upload
  for(int i=0;i<codeSize;i++)
  {
      ptr[i]=(uint32_t)code[i]; // 16->32, instructionis 16 bits, reg is 32
  }
  return true;
}
/**
 */
bool rpPIO::setSpeed(uint32_t fq)
{
    uint32_t fq_in = clock_get_hz(clk_sys);
    uint32_t intdiv = fq_in/fq;
// JUST FOR TESTING
#warning FOR TEST ONLY
    ENGINE()->PIO_SM[0].CLKDIV=LN_RP_PIO_SM_CLKDIV( intdiv, 0);
    return false;
}
/**
 * THIS IS INCCORECT TODO **FIXME**
 */
bool rpPIO::configure(uint32_t startPin, uint32_t nb_pin, uint32_t start_set_pin )
{
  #warning FIXME TEMPORARY
  uint32_t pin_ctrol = 
    LN_RP_PIO_SM_PINCTRL_OUT_BASE_BIT(startPin) + // assume base = set
    LN_RP_PIO_SM_PINCTRL_SET_BASE_BIT(startPin) +   
    LN_RP_PIO_SM_PINCTRL_SETSET_BASE_BIT(start_set_pin)  +
    LN_RP_PIO_SM_PINCTRL_IN_BASE_BIT(startPin)    +
    LN_RP_PIO_SM_PINCTRL_OUT_COUNT_BIT(nb_pin)  +
    LN_RP_PIO_SM_PINCTRL_SET_COUNT_BIT(nb_pin)  +
    LN_RP_PIO_SM_PINCTRL_SIDESET_COUNT_BIT(nb_pin) +
    0 ;
  ENGINE()->PIO_SM[0].PINCTRL= pin_ctrol ;
  ENGINE()->PIO_SM[0].SHIFTCTRL= 
      LN_RP_PIO_SM_SHIFTCTRL_OUT_SHIFTDIR + // shift output to the right, input to the left
      0 ;
  return true;
}
/**
 *
 */
bool rpPIO::execute()
{
  ENGINE()->PIO_SM[0].INSTR=_offset; // set PC
  startPIO( ENGINE(), 0);
  return false;
}
/**
 *
 */
bool rpPIO::stop()
{
  stopPIO( ENGINE(), 0);
  return false;
}
// -- EOF
