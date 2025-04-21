#include "Common.h"

#define CALENDAR_YEAR 365.25

double CurrentEpoch();
double JulianDay(uint32_t Y, uint32_t M, uint32_t D, uint32_t h, uint32_t m, uint32_t s);
double tleEpochToJJ(uint16_t Y, double day);