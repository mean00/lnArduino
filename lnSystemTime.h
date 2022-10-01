/**

 */
#pragma once
uint32_t lnGetUs();
uint64_t lnGetUs64();
void     lnDelayUs(uint32_t wait); // us
void     lnDelay(uint32_t wait); // ms
#define lnDelayMs lnDelay
uint32_t lnGetMs();
