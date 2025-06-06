#pragma once
#include "stdint.h"

// it's not really const
// but it's thread safe by design

typedef struct
{
    void *dummy;
} lnfast_event_group_c;

lnfast_event_group_c *lnfast_event_group_create();
void lnfast_event_group_delete(const lnfast_event_group_c *);

void lnfast_event_group_takeOwnership(const lnfast_event_group_c *inst);
void lnfast_event_group_set_events(const lnfast_event_group_c *inst, uint32_t event);
uint32_t lnfast_event_group_wait_events(const lnfast_event_group_c *inst, uint32_t mask, int32_t timeout);
uint32_t lnfast_event_group_read_events(const lnfast_event_group_c *inst, uint32_t mask);

// EOF