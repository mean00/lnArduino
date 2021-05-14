
#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"

LN_RCU *arcu=(LN_RCU *)RCU;
/**
 * 
 * @param periph
 */
void lnPeripherals::reset(const Peripherals periph)
{
    
}
/**
 * 
 * @param periph
 */
void lnPeripherals::enable(const Peripherals periph)
{
    
}
/**
 * 
 * @param periph
 */
void lnPeripherals::disable(const Peripherals periph)
{
    
}

uint32_t clockApb1,clockApb2;

/**
 * 
 * @param periph
 * @return 
 */
uint32_t lnPeripherals::getClock(const Peripherals periph)
{
    switch(periph)
    {
        case pUART0:
        case pSPI0:
                    return clockApb2;
        default:
                    return clockApb1;
    }
}

// EOF
