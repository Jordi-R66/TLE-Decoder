#include "Common.h"

typedef struct tm tm;

typedef struct Date {
	uint16_t Year;
	uint16_t DayOfYear; // [0; 365]

	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
} Date;

#define CALENDAR_YEAR 365.25

double CurrentEpoch();
double JulianDay(uint32_t Y, uint32_t M, uint32_t D, uint32_t h, uint32_t m, uint32_t s);
double tleEpochToJJ(uint16_t Y, double day);

Date JJToReadable(double JJ);