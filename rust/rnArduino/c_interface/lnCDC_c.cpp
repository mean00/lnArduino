
#include "lnArduino.h"
#include "include/lnUsbStack.h"
#include "include/lnUsbCDC.h"
#include "lnCDC_c.h"
#define WRAPME(x) ((lnUsbCDC *)x)
/*

*/
lncdc_c *   lncdc_create(uint32_t instance)
{
    return (lncdc_c *)(new lnUsbCDC((int)instance));
}
/*

*/

void        lncdc_delete(lncdc_c *instance)
{
    delete WRAPME(instance);
    instance=NULL;    
}

/*

*/

int         lncdc_read(lncdc_c *instance, uint8_t *buffer, int maxSize)
{
    return WRAPME(instance)->read(buffer,maxSize);
}
/*

*/

int         lncdc_write(lncdc_c *instance, const uint8_t *buffer, int maxSize)
{
    return WRAPME(instance)->write(buffer,maxSize);
}
/*

*/

void        lncdc_flush(lncdc_c *instance)
{
    return WRAPME(instance)->flush();
}
/*

*/

void        lncdc_clear_input_buffers(lncdc_c *instance)
{
    return WRAPME(instance)->clear_input_buffers();
}
/*

*/

void        lncdc_set_event_handler(lncdc_c *instance, cdc_event_handler *handler, void *cookie)
{
      return WRAPME(instance)->setEventHandler(( lnUsbCDC::lnUsbCDCEventsHandler *)handler,cookie);
}
/*

*/

          
// EOF