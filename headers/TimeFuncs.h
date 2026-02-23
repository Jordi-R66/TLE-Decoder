#pragma once

#include "Common.h"
#include "TleParser.h"
#include "../libs/Calendar/Converters.h"

#define DELTA_TIME_STRING_LENGTH 20
#define DATE_STRING_LENGTH 30

time_t getEpochTimestampFromTLE(TLE tle);

/**
 * @brief Generates a string representing the date et time corresponding to a timestamp, the format is `YYYY-MM-DD HH:mm:SS UTC`
 *
 * @param timestamp Unix timestamp (UTC)
 * @param string The string to write to
 */
void timestampToDateString(time_t timestamp, char string[DATE_STRING_LENGTH]);

void deltaTimeToString(time_t deltaTime, char string[DELTA_TIME_STRING_LENGTH]);