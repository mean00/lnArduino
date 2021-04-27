/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
#if 1
    void LoggerInit();
    void Logger(const char *fmt...);
    void debugLogger(const char *fmt...);
#else
    #define LoggerInit() {}
    #define Logger(...) {}
#endif
