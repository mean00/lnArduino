
#include "lnArduino.h"
extern "C"
{
#include "lnFast_EventGroup_c.h"
}
#include "lnFreeRTOS_pp.h"
#define WRAPME(x) ((lnFastEventGroup *)x)

lnfast_event_group_c *lnfast_event_group_create()
{
    return (lnfast_event_group_c *)new lnFastEventGroup;
}
void lnfast_event_group_delete(const lnfast_event_group_c *f)
{
    delete WRAPME(f);
}

void lnfast_event_group_takeOwnership(const lnfast_event_group_c *inst)
{
    WRAPME(inst)->takeOwnership();
}
void lnfast_event_group_set_events(const lnfast_event_group_c *inst, uint32_t event)
{
    WRAPME(inst)->setEvents(event);
}
uint32_t lnfast_event_group_wait_events(const lnfast_event_group_c *inst, uint32_t mask, int32_t timeout)
{
    return WRAPME(inst)->waitEvents(mask, timeout);
}
uint32_t lnfast_event_group_read_events(const lnfast_event_group_c *inst, uint32_t mask)
{
    return WRAPME(inst)->readEvents(mask);
}
// EOF
