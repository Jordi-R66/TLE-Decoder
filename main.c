#include "headers/DynamicPhase.h"

TLE initPhase(char* filename, uint32_t noradId) {
	FILE* fp = fopen(filename, "r");
	TLE tle = GetSingleTLE(fp, noradId);
	fclose(fp);

	return tle;
}

int main(int argc, char** argv) {
	char* filename = argv[1];
	uint32_t noradId = strint(argv[2]);

	TLE tle = initPhase(filename, noradId);
	StaticValues staticVals = staticPhase(tle);

	time_t epoch_ts = getEpochTimestampFromTLE(tle);
	time_t now = time(NULL);

	char epoch_string[DATE_STRING_LENGTH], now_string[DATE_STRING_LENGTH];

	timestampToDateString(epoch_ts, epoch_string);
	timestampToDateString(now, now_string);

	printf("Timestamp epoch : %ld (%s)\n", epoch_ts, epoch_string);
	printf("Timestamp actuel : %ld (%s)\n", now, now_string);

	printf("DeltaT : %ld\n", now - epoch_ts);

	return 0;
}