#pragma once
#include "Common.h"

typedef struct tm tm;

typedef struct Date {
	uint16_t Year;
	uint8_t Month;
	uint8_t Day;
	double DayOfYear; // [0; 365] + Fraction of day

	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
} Date;

#define CALENDAR_YEAR 365.25
#define EARTH_DAY_LENGTH 86400.0
//86164.096

/**
 * @brief Computes the time delta between two dates
 * 
 * @param A Latest date
 * @param B Oldest date
 * @return double time delta in seconds 
 */
double timeDelta(Date A, Date B);
Date CurrentDate();
Date tleToDate(uint16_t EPOCH_YEAR, double EPOCH_DAY);
double JulianDay(uint32_t Y, uint32_t M, uint32_t D, uint32_t h, uint32_t m, uint32_t s);
Date tleToDate(uint16_t EPOCH_YEAR, double EPOCH_DAY);

Date JJToReadable(double JJ);