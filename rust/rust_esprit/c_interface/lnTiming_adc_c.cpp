#include "lnTiming_adc_c.h"
#include "lnADC.h"
#define WRAP() ((lnTimingAdc *)instance)

ln_timing_adc_c *ln_timing_adc_create(int instance)
{
    return (ln_timing_adc_c *)new lnTimingAdc(instance);
}
bool ln_timing_adc_delete(ln_timing_adc_c *instance)
{
    delete WRAP();
    return true;
}
bool ln_timing_adc_set_source(ln_timing_adc_c *instance, int timer, int channel, int fq, int nbPins, const lnPin *pin)
{
    return WRAP()->setSource(timer, channel, fq, nbPins, pin);
}
bool ln_timing_adc_multi_read(ln_timing_adc_c *instance, int nbSamplePerChannel, uint16_t *output)
{
    return WRAP()->multiRead(nbSamplePerChannel, output);
}
