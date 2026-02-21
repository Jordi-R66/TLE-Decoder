#pragma once
#include "Common.h"

#include "TleParser.h"
#include "../libs/Calendar/Converters.h"

time_t getEpochTimestampFromTLE(TLE tle);
