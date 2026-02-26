#include "Aggreg.h"

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

AggregValues getAggregAtTimestamp(char* filename, uint32_t norad_id, time_t timestamp) {
	AggregValues output = { 0 };

	output.timestamp = timestamp;

	TLE tle = initPhase(filename, norad_id);
	output.tle = tle;

	StaticValues init = staticPhase(tle);
	output.init = init;

	DynamicValues at_instant = dynamicPhase(tle, init, timestamp);
	output.values_at_time = at_instant;

	return output;
}

void changeAggregTimestamp(AggregValues* aggreg, time_t timestamp) {
	aggreg->timestamp = timestamp;
	aggreg->values_at_time = dynamicPhase(aggreg->tle, aggreg->init, timestamp);
}


void printValues(FILE* stream, AggregValues vals) {
	TLE tle = vals.tle;
	StaticValues init = vals.init;
	DynamicValues instant = vals.values_at_time;

	time_t instant_ts = vals.timestamp;

	timestampToDateString(init.epoch_timestamp, epoch_string);
	periodToString(init.T, period_string);

	timestampToDateString(instant_ts, instant_string);
	deltaTimeToString(instant.deltaTime, age_string);

	fprintf(stream, "Object name : %s\n", tle.name);

	fprintf(stream, "----------------------------------- TLE -----------------------------------\n");

	fprintf(stream, "NORAD ID : %u%c\n", tle.NORAD_ID, tle.Classification);
	fprintf(stream, "COSPAR : %u %03hu %s\n", tle.COSPAR_YR, tle.COSPAR_LN, tle.COSPAR_ID);
	fprintf(stream, "EPOCH : %s\n", epoch_string);
	fprintf(stream, "TLE AGE : %s\n", age_string);
	fprintf(stream, "(MEAN MOTION)' = %.4e\n", tle.FIRST_DERIV_MEAN_MOTION);
	fprintf(stream, "(MEAN MOTION)'' = %.4e\n", tle.SECOND_DERIV_MEAN_MOTION);
	fprintf(stream, "B* = %.4e\n", tle.B_STAR);

	fprintf(stream, "\n");

	fprintf(stream, "INCLINATION : %.4lf degs\n", tle.Inclination);
	fprintf(stream, "LONGITUDE OF ASC. NODE : %.4lf degs\n", tle.AscNodeLong);
	fprintf(stream, "LONGITUDE OF PERIAPSIS : %.4lf degs\n", tle.AscNodeLong + tle.PeriArg);
	fprintf(stream, "ECCENTRICITY : %.7lf\n", tle.Eccentricity);
	fprintf(stream, "ARG. OF PERIAPSIS : %.4lf degs\n", tle.PeriArg);
	fprintf(stream, "MEAN ANOMALY : %.4lf degs\n", tle.MeanAnomaly);
	fprintf(stream, "MEAN MOTION : %.8lf rev/day\n", tle.MeanMotion);

	fprintf(stream, "--------------------------------- RESULTS ---------------------------------\n");

	fprintf(stream, "ORBITAL PERIOD : %.4lf secs (%s)\n", init.T, period_string);
	fprintf(stream, "SEMI MAJOR AXIS : %.0lf m\n", init.a);
	fprintf(stream, "APOAPSIS : %.0lf m | PERIAPSIS : %.0lf m | EPOCH : %.0lf m\n", init.apo_alt, init.peri_alt, init.epoch_alt);
	fprintf(stream, "SPEED @ AP : %.4lf m/s | PE : %.4lf m/s | EP : %.4lf m/s\n", init.apo_spd, init.peri_spd, init.epoch_spd);

	fprintf(stream, "-------------------------------- CURRENTLY --------------------------------\n");

	fprintf(stream, "DATE : %s\n", instant_string);
	fprintf(stream, "X Coord : %.2lf m\n", instant.coords_3d.x);
	fprintf(stream, "Y Coord : %.2lf m\n", instant.coords_3d.y);
	fprintf(stream, "Z Coord : %.2lf m\n", instant.coords_3d.z);
	fprintf(stream, "\n");
	fprintf(stream, "LATITUDE : %.5lf\n", instant.geo_coords.latitude);
	fprintf(stream, "LONGITUDE : %.5lf\n", instant.geo_coords.longitude);
	fprintf(stream, "ALTITUDE : %.0lf m\n", instant.geo_coords.altitude);
	fprintf(stream, "\n");
	fprintf(stream, "VELOCITY : %.2lf m/s\n", instant.speed);

	fprintf(stream, "---------------------------------------------------------------------------\n");
}