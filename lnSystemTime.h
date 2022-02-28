/**

 */
#pragma once
uint32_t lnGetUs();
uint64_t lnGetUs64();
void     lnDelayUs(int wait); // us
void     lnDelay(int wait); // ms
#define lnDelayMs lnDelay
uint32_t lnGetMs();
