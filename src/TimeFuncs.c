#include "TimeFuncs.h"

time_t getEpochTimestampFromTLE(TLE tle) {
	time_t output;
	double epochAsNorad = (double)(((uint16_t)tle.EPOCH_YR) * 1000) + tle.EPOCH;

	CompleteTimeStruct temp = generalConverter(
		(TimeStruct) {
		.Norad = epochAsNorad
	}, NORAD_TIME, UNIX_TIME
	);

	output = temp.TIMESTAMP;

	return output;
}

/**
 * @brief Generates a string representing the date et time corresponding to a timestamp, the format is `YYYY-MM-DD HH:mm:SS UTC`
 *
 * @param timestamp Unix timestamp (UTC)
 * @param string The string to write to
 */
void timestampToDateString(time_t timestamp, char string[DATE_STRING_LENGTH]) {
	CompleteTimeStruct temp = generalConverter(
		(TimeStruct) {
		.TIMESTAMP = timestamp
	}, UNIX_TIME, GREGORIAN_CAL
	);

	memset(string, 0, DATE_STRING_LENGTH);

	DateStruct date_ = temp.GregDate;
	TimeOfDay time_ = temp.timeOfDay;

	uint16_t Year = date_.YEAR;
	uint8_t Month = date_.MONTH,
		Day = date_.DAY,
		Hour = time_.HOUR,
		Minute = time_.MINUTE,
		Seconds = time_.SECONDS;

	sprintf(string, "%04hu-%02u-%02u %02u:%02u:%02u UTC", Year, Month, Day, Hour, Minute, Seconds);
}

void periodToString(double period, char string[DELTA_TIME_STRING_LENGTH]) {
	memset(string, 0, DELTA_TIME_STRING_LENGTH);

	time_t deltaTime = (time_t)(period * 1000.0);

	uint16_t Days, Millis;
	uint8_t Hours, Minutes, Seconds;

	Days = deltaTime / 86400000;
	deltaTime %= 86400000;

	Hours = deltaTime / 3600000;
	deltaTime %= 3600000;

	Minutes = deltaTime / 60000;
	deltaTime %= 60000;

	Seconds = deltaTime / 1000;
	deltaTime %= 1000;

	Millis = deltaTime;

	sprintf(string, "%hud %uh %um %u.%03us", Days, Hours, Minutes, Seconds, Millis);

	return;
}

void deltaTimeToString(time_t deltaTime, char string[DELTA_TIME_STRING_LENGTH]) {
	memset(string, 0, DELTA_TIME_STRING_LENGTH);

	uint16_t Days;
	uint8_t Hours, Minutes, Seconds;

	Days = deltaTime / 86400;
	deltaTime %= 86400;

	Hours = deltaTime / 3600;
	deltaTime %= 3600;

	Minutes = deltaTime / 60;
	deltaTime %= 60;

	Seconds = deltaTime;

	sprintf(string, "%hud %02uh %02um %02us", Days, Hours, Minutes, Seconds);

	return;
}