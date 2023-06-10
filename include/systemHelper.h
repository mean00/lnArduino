/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
extern "C"
{
    void deadEnd(int code);
}

#define ENTER_CRITICAL vPortEnterCritical
#define EXIT_CRITICAL vPortExitCritical

extern "C" void ENTER_CRITICAL(void);
extern "C" void EXIT_CRITICAL(void);
