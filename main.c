#include "headers/utils.h"
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

	char now_string[DATE_STRING_LENGTH];

	while (true) {
		time_t current_ts = time(NULL);
		DynamicValues dynVals = dynamicPhase(tle, staticVals, current_ts);

		timestampToDateString(current_ts, now_string);

		clear_screen();
		printf("%s\nnu: %.5e rads\nDist: %.15f meters\n", now_string, dynVals.true_ano, dynVals.distanceToFocal);

		sleep_ms(20); // Pause 20ms soit 50Hz
	}

	return 0;
}