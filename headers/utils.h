#pragma once

#include "Common.h"

#ifdef _WIN32
    #include <windows.h>
#endif

void sleep_ms(unsigned int milliseconds);
void sleep_ns(uint64_t nanoseconds);
void sleep_hz(unsigned int hz);

void clear_screen(void);
