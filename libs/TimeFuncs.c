#include "TimeFuncs.h"

double CurrentEpoch() {
	time_t current_time = time(NULL);

	struct tm* utc = gmtime(&current_time);

	uint32_t Y = utc->tm_year + 1900;
	uint32_t M = utc->tm_mon + 1;
	uint32_t D = utc->tm_mday;
	uint32_t h = utc->tm_hour;
	uint32_t m = utc->tm_min;
	uint32_t s = utc->tm_sec;

	// printf("YEAR : %u\nMONTH : %u\n", Y, M);

	double Epoch = JulianDay(Y, M, D, h, m, s);
	return Epoch;
}

double JulianDay(uint32_t Y, uint32_t M, uint32_t D, uint32_t h, uint32_t m, uint32_t s) {
	double Q = (double)D + ((double)((h-12+12)*3600 + m*60 + s))/86400.0;

	double JJ;

	if (M <= 2) {
		Y--;
		M += 12;
	}

	uint8_t S = Y/100;
	uint8_t B = 2 - S + S/4;

	JJ = (uint32_t)(365.25 * Y) + (uint32_t)(30.6001*(M+1)) + Q + B + 1720994.5;

	return JJ;
}