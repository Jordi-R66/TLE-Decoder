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