#include "TimeFuncs.h"

int64_t ENT(double x) {
	return (int64_t)floor(x);
}

double timeDelta(Date A, Date B) {
	uint16_t latest_year = A.Year;
	double latest_day = (double)A.DayOfYear + (double)(A.Hour) / 24.0 + (double)(A.Minute) / 1440.0 + (double)(A.Second) / EARTH_DAY_LENGTH + 1.0;

	uint16_t oldest_year = B.Year;
	double oldest_day = (double)B.DayOfYear + (double)(B.Hour) / 24.0 + (double)(B.Minute) / 1440.0 + (double)(B.Second) / EARTH_DAY_LENGTH + 1.0;

	double DeltaTime = (((double)(latest_year - oldest_year) * CALENDAR_YEAR) + (double)(latest_day - oldest_day)) * EARTH_DAY_LENGTH;
	return DeltaTime;
}

Date CurrentDate() {
	time_t current_time = time(NULL);

	tm* utc = gmtime(&current_time);

	uint32_t Y = utc->tm_year + 1900;
	uint8_t M = utc->tm_mon + 1;
	uint8_t D = utc->tm_mday;
	uint8_t h = utc->tm_hour;
	uint8_t m = utc->tm_min;
	uint8_t s = utc->tm_sec;

	Date Epoch = {Y, D, M, utc->tm_yday, h, m, s};

	return Epoch;
}

double JulianDay(uint32_t Y, uint32_t M, uint32_t D, uint32_t h, uint32_t m, uint32_t s) {
	double F = h * 3600.0 + m * 60 + s * 1.0;
	F /= 86400.0;

	double Q = (double)D + F;

	if (M == 1 || M == 2) {
		Y -= 1;
		M += 12;
	}

	double S = ENT((double)Y / 100.0);
	double B = 2 - S + ENT(S / 4.0);

	return (ENT(365.25 * Y) + ENT(30.6001 * (M + 1)) + Q + B + 1720994.5 + 0.5);
}

int64_t JulianDayInt(uint32_t Y, uint32_t M, uint32_t D) {
	return ENT(JulianDay(Y, M, D, 0, 0, 0));
}

Date tleToDate(uint16_t EPOCH_YEAR, double EPOCH_DAY) {
	Date output;

	output.Year = EPOCH_YEAR;
	output.DayOfYear = (uint16_t)ENT(EPOCH_DAY);

	double dayFraction = EPOCH_DAY - (double)output.DayOfYear;

	double h = dayFraction * 24;
	double m = (h - ENT(h)) * 60;
	double s = (m - ENT(m)) * 60;

	output.Hour = (uint8_t)ENT(h);
	output.Minute = (uint8_t)ENT(m);
	output.Second = (uint8_t)ENT(s);

	int64_t JJ_YEAR = JulianDayInt(EPOCH_YEAR, 1, 1);
	int64_t JJ_EPOCH = JJ_YEAR + output.DayOfYear - 1;

	output.DayOfYear = JJToReadable(JJ_EPOCH).DayOfYear;

	return output;
}

Date JJToReadable(double JJ) {
	double Z, F;

	Z = (double)ENT(JJ);
	F = JJ - Z;

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
	uint8_t Minute = (uint8_t)ENT(1440.0 * (F - (double)Hour/24.0));
	uint8_t Second = ENT(86400.0 * (F - (double)Hour/24.0 - (double)Minute/1440));

	uint16_t DayOfYear = ENT(JJ) - JulianDayInt(Year, 1, 1) + 1;

	Date date = {Year, Month, Day, DayOfYear, Hour, Minute, Second};

	return date;
}
