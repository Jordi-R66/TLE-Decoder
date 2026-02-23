#include "headers/utils.h"
#include "headers/DynamicPhase.h"

static char epoch_string[DATE_STRING_LENGTH] = {0};
static char instant_string[DATE_STRING_LENGTH] = {0};

static char age_string[DELTA_TIME_STRING_LENGTH] = {0};

TLE initPhase(char* filename, uint32_t noradId) {
	FILE* fp = fopen(filename, "r");
	TLE tle = GetSingleTLE(fp, noradId);
	fclose(fp);

	return tle;
}

void printValues(TLE tle, StaticValues init, DynamicValues instant) {
	time_t instant_ts = init.epoch_timestamp + instant.deltaTime;

	timestampToDateString(instant_ts, instant_string);
	deltaTimeToString(instant.deltaTime, age_string);

	printf("%lu\n", instant.deltaTime);

	printf("Object name : %s\n", tle.name);
	printf("\n");
	printf("NORAD ID : %u%c\n", tle.NORAD_ID, tle.Classification);
	printf("COSPAR : %u %hu %s\n", tle.COSPAR_YR, tle.COSPAR_LN, tle.COSPAR_ID);
	printf("EPOCH : %s\n", epoch_string);
	printf("TLE AGE : %s\n", age_string);
	printf("(MEAN MOTION)' = \n");
	printf("(MEAN MOTION)'' = \n");
	printf("B* = \n");
}

int main(int argc, char** argv) {
	char* filename = argv[1];
	uint32_t noradId = strint(argv[2]);

	TLE tle = initPhase(filename, noradId);
	StaticValues staticVals = staticPhase(tle);

	timestampToDateString(staticVals.epoch_timestamp, epoch_string);

	while (true) {
		time_t current_ts = time(NULL);
		DynamicValues dynVals = dynamicPhase(tle, staticVals, current_ts);

		clear_screen();

		printValues(tle, staticVals, dynVals);

		sleep_ms(20); // Pause 20ms soit 50Hz
	}

	return 0;
}