#include "headers/utils.h"
#include "headers/DynamicPhase.h"

static char epoch_string[DATE_STRING_LENGTH] = { 0 };
static char instant_string[DATE_STRING_LENGTH] = { 0 };

static char age_string[DELTA_TIME_STRING_LENGTH] = { 0 };
static char period_string[DELTA_TIME_STRING_LENGTH] = { 0 };

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

	printf("Object name : %s\n", tle.name);

	printf("----------------------------------- TLE -----------------------------------\n");

	printf("NORAD ID : %u%c\n", tle.NORAD_ID, tle.Classification);
	printf("COSPAR : %u %hu %s\n", tle.COSPAR_YR, tle.COSPAR_LN, tle.COSPAR_ID);
	printf("EPOCH : %s\n", epoch_string);
	printf("TLE AGE : %s\n", age_string);
	printf("(MEAN MOTION)' = %.4e\n", tle.FIRST_DERIV_MEAN_MOTION);
	printf("(MEAN MOTION)'' = %.4e\n", tle.SECOND_DERIV_MEAN_MOTION);
	printf("B* = %.4e\n", tle.B_STAR);

	printf("\n");

	printf("INCLINATION : %.4lf degs\n", tle.Inclination);
	printf("LONGITUDE OF ASC. NODE : %.4lf degs\n", tle.AscNodeLong);
	printf("LONGITUDE OF PERIAPSIS : %.4lf degs\n", tle.AscNodeLong + tle.PeriArg);
	printf("ECCENTRICITY : %.7lf\n", tle.Eccentricity);
	printf("ARG. OF PERIAPSIS : %.4lf degs\n", tle.PeriArg);
	printf("MEAN ANOMALY : %.4lf degs\n", tle.MeanAnomaly);
	printf("MEAN MOTION : %.8lf rev/day\n", tle.MeanMotion);

	printf("--------------------------------- RESULTS ---------------------------------\n");

	printf("ORBITAL PERIOD : %.4lf secs (%s)\n", init.T, period_string);
	printf("SEMI MAJOR AXIS : %.0lf m\n", init.a);
	printf("APOAPSIS : %.0lf m | PERIAPSIS : %.0lf m | EPOCH : %.0lf m\n", 0.0, 0.0, 0.0);
	printf("SPEED @ AP : %.4lf m/s | PE : %.4lf m/s | EP : %.4lf m/s\n", 0.0, 0.0, 0.0);

	printf("-------------------------------- CURRENTLY --------------------------------\n");

	printf("DATE : %s\n", instant_string);
	printf("X Coord : %.0lf m\n", instant.coords_2d.x);
	printf("Y Coord : %.0lf m\n", instant.coords_2d.y);
	printf("Z Coord : %.0lf m\n", 0.0);
	printf("ALTITUDE : %.0lf m\n", instant.distanceToFocal);
	printf("VELOCITY : %.2lf m/s\n", instant.speed);

	printf("---------------------------------------------------------------------------\n");
}

int main(int argc, char** argv) {
	char* filename = argv[1];
	uint32_t noradId = strint(argv[2]);

	TLE tle = initPhase(filename, noradId);
	StaticValues staticVals = staticPhase(tle);

	timestampToDateString(staticVals.epoch_timestamp, epoch_string);
	periodToString(staticVals.T, period_string);

	while (true) {
		time_t current_ts = time(NULL);
		DynamicValues dynVals = dynamicPhase(tle, staticVals, current_ts);

		clear_screen();

		printValues(tle, staticVals, dynVals);

		sleep_ms(17); // Pause 17ms soit ~60Hz
	}

	return 0;
}