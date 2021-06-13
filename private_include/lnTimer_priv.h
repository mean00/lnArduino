#pragma once
#pragma once
#include "lnIRQ.h"
#include "lnPeripheral_priv.h"

#if 0
struct LN_ECLIC_irqx
{
    uint8_t ip;
    uint8_t ie;
    uint8_t attr;
    uint8_t control;
};

typedef volatile LN_ECLIC_irqx LN_ECLIC_irq;

uint32_t     *eclicCfg=(uint32_t *)(LN_ECLIC_ADR);
uint32_t     *eclicInfo=(uint32_t *)(LN_ECLIC_ADR+4); // Read only
uint8_t      *eclicMth=(uint8_t *)(LN_ECLIC_ADR+11);


//List of interrupts
LN_ECLIC_irq *eclicIrqs=(LN_ECLIC_irq *)(LN_ECLIC_ADR+0x1000);

#endif
