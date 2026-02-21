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
