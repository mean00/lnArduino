#include "lnArduino.h"
#include "ln_rp_pio_priv.h"
#include "ln_rp_pio.h"

static int consumedPioSpace[2]={0,0};
static const LN_RP_PIO *rpiox[2] = { (LN_RP_PIO *) LN_PIO0_BASE_ADR , (LN_RP_PIO *) LN_PIO1_BASE_ADR };
#define ENGINE() ((LN_RP_PIO *)rpiox[_unit])


static void resetPIO( LN_RP_PIO *rpio,uint32_t sm )
{
  uint32_t ctrl = rpio->PIO_CTRL;
  ctrl&=~LN_RP_PIO_CTRL_ENABLE_BIT(sm); // disable all SM

  rpio->PIO_CTRL=ctrl;


}

/**
 *
 */
rpPIO::rpPIO(uint32_t unit)
{
  _unit = unit;
  _codeSize=-1;
  _offset=-1;
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
bool rpPIO::uploadCode(uint32_t codeSize, const uint8_t *code)
{
  xAssert(consumedPioSpace[_unit]==0);
  xAssert(codeSize<= LN_RP_PIO_MAX_PROGRAM_SPACE);
  consumedPioSpace[_unit]=codeSize;
  _codeSize=codeSize;
  _offset =0;
  resetPIO(ENGINE(),0);
  return true;
}
/**
 *
 */
bool rpPIO::execute()
{
  xAssert(0);
  return false;

}
/**
 *
 */
bool rpPIO::stop()
{
  xAssert(0);
  return false;

}
  /*
        int _sm;
        int _offset;
        int _codeSize;
*/
