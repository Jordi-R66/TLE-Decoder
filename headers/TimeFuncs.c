#include "TimeFuncs.h"

int64_t ENT(double x) {
	return (int64_t)floor(x);
}

double timeDelta(Date A, Date B) {
	uint16_t latest_year = A.Year;
	double latest_day = (double)A.DayOfYear + (double)(A.Hour) / 24.0 + (double)(A.Minute) / 1440.0 + (double)(A.Second) / EARTH_DAY_LENGTH;

	uint16_t oldest_year = B.Year;
	double oldest_day = (double)B.DayOfYear + (double)(B.Hour) / 24.0 + (double)(B.Minute) / 1440.0 + (double)(B.Second) / EARTH_DAY_LENGTH;

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

	Date Epoch = { Y, M, D, utc->tm_yday + 1, h, m, s };

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

	// 1. Resolve 2-Digit Year Windowing (US Space Force Standard)
	if (EPOCH_YEAR < 100) {
		EPOCH_YEAR += (EPOCH_YEAR >= 57) ? 1900 : 2000;
	}
	output.Year = EPOCH_YEAR;

	// 2. Extract Day of Year and fractional time
	uint16_t dayOfYearInt = (uint16_t)ENT(EPOCH_DAY);
	double dayFraction = EPOCH_DAY - (double)dayOfYearInt;

	// Assign to the double field in your struct to preserve the exact epoch
	output.DayOfYear = EPOCH_DAY;

	// 3. Calculate Hours, Minutes, Seconds with rounding protection
	double h = dayFraction * 24.0;
	double m = (h - ENT(h)) * 60.0;
	double s = (m - ENT(m)) * 60.0;

	output.Hour = (uint8_t)ENT(h);
	output.Minute = (uint8_t)ENT(m);

	// Add 0.5 to prevent floating-point truncation from stealing a second
	output.Second = (uint8_t)ENT(s + 0.5);

	// Handle micro-edge-case: rounding pushes seconds to 60
	if (output.Second == 60) {
		output.Second = 0;
		output.Minute++;
		if (output.Minute == 60) {
			output.Minute = 0;
			output.Hour++;
		}
	}

	// 4. Calculate Month and Day using the Meeus algorithm
	// We calculate the Midnight Julian Date of the target day directly.
	int64_t JJ_YEAR = JulianDayInt(output.Year, 1, 1);
	double Z = (double)(JJ_YEAR + dayOfYearInt - 1);

	double a = ENT((Z - 1867216.25) / 36524.25);
	double S = Z + 1.0 + a - ENT(a / 4.0);

	double B = S + 1524.0;
	double C = ENT((B - 122.1) / CALENDAR_YEAR);
	double D = ENT(CALENDAR_YEAR * C);
	double E = ENT((B - D) / 30.6001);

	output.Day = (uint8_t)(B - D - ENT(30.6001 * E));
	output.Month = (uint8_t)(E >= 14.0 ? E - 13.0 : E - 1.0);

	return output;
}

Date JJToReadable(double JJ) {
	double Z, F;

	Z = (double)ENT(JJ);
	F = JJ - Z;

	double a = ENT((Z - 1867216.25) / 36524.25);
	double S = Z + 1 + a - (double)ENT(a / 4.0);

	double B = S + 1524;
	double C = (double)ENT((B - 122.1) / CALENDAR_YEAR);
	double D = (double)ENT(CALENDAR_YEAR * C);
	double E = (double)ENT((B - D) / 30.6001);

	uint8_t Day = (uint8_t)(B - D - (double)ENT(30.6001 * E));
	uint8_t Month = E >= 14 ? E - 13 : E - 1;
	uint16_t Year = Month > 2 ? C - 4716 : C - 4715;

	uint8_t Hour = (uint8_t)ENT(F * 24.0);
	uint8_t Minute = (uint8_t)ENT(1440.0 * (F - (double)Hour / 24.0));
	uint8_t Second = ENT(86400.0 * (F - (double)Hour / 24.0 - (double)Minute / 1440));

	uint16_t DayOfYear = ENT(JJ) - JulianDayInt(Year, 1, 1) + 1;

	Date date = { Year, Month, Day, DayOfYear, Hour, Minute, Second };

	return date;
}
