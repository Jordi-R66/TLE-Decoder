#pragma once

#include "Aggreg.h"

void realtimeLoop(char* filename, uint32_t noradId, AggregWriter writingFunc, FILE* outputStream);
void sweepTimestamps(char* filename, uint32_t noradId, AggregWriter wwritingFunc, FILE* outputStream, time_t start, time_t stop, time_t step);