/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
#if 1
void LoggerInit();
void debugLogger(const char *fmt...);
#else
#define LoggerInit()                                                                                                   \
    {                                                                                                                  \
    }
#define Logger(...)                                                                                                    \
    {                                                                                                                  \
    }
#endif

extern "C" void Logger(const char *fmt...);
extern "C" int Logger_C(const char *fmt, ...);
extern "C" void Logger_chars(int n, const char *data);