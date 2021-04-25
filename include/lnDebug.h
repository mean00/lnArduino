#pragma once
#if 1
    void LoggerInit();
    void Logger(const char *fmt...);
    void debugLogger(const char *fmt...);
#else
    #define LoggerInit() {}
    #define Logger(...) {}
#endif
