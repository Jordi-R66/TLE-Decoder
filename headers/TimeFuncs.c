#include "TimeFuncs.h"

int64_t ENT(double x) {
	return (int64_t)floor(x);
}

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
	double Q = (double)D + ((double)((h - 12 + 12) * 3600 + m * 60 + s)) / 86400.0;

	double JJ;

	if (M <= 2) {
		Y--;
		M += 12;
	}

	uint8_t S = Y / 100;
	uint8_t B = 2 - S + S / 4;

	JJ = (uint32_t)(365.25 * Y) + (uint32_t)(30.6001 * (M + 1)) + Q + B + 1720994.5;

	return JJ;
}

double tleEpochToJJ(uint16_t Y, double day) {
	double JJ_REF = JulianDay(1900, 1, 1, 0, 0, 0);

	return JJ_REF + (Y - 1900) * CALENDAR_YEAR + day;
}

Date JJToReadable(double JJ) {
	double Z, F;

	Z = (double)ENT(JJ);
	F = JJ - Z;

	if (Z < 2299161) {
		exit(EXIT_FAILURE);
	}

	double a = ENT( (Z - 1867216.25)  / 36524.25);
	double S = Z + 1 + a - (double)ENT(a/4.0);

	double B = S + 1524;
	double C = (double)ENT( (B - 122.1) / CALENDAR_YEAR);
	double D = (double)ENT(CALENDAR_YEAR * C);
	double E = (double)ENT((B-D)/30.6001);

	uint8_t Day = (uint8_t)(B - D - (double)ENT(30.6001 * E));
	uint8_t Month = E >= 14 ? E - 13 : E - 1;
	uint16_t Year = Month > 2 ? C - 4716 : C - 4715;

	uint8_t Hour = (uint8_t)ENT(F * 24.0);
	uint8_t Minute = (uint8_t)ENT((ENT(F * 24.0) - Hour) * 60.0);
	uint8_t Second = (uint8_t)((ENT((ENT(F * 24.0) - Hour) * 60.0) - Minute) * 60.0);

	Date date = {Year, Month, Day, Hour, Minute, Second};

	return date;
}
