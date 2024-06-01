#include <time.h>
#include <windows.h>

#include "libs/TLE_PARSER.h"
#include "libs/OrbMaths.h"
#include "libs/TLE_Files.h"
#include "libs/TimeFuncs.h"
#include "libs/Algos.h"

#include "libs/Common.h"

#define CHECKSUM_MODULO 10

int32_t checksum_algorithm(string *line, int32_t modulo) {

	int32_t checksum = 0;
	int32_t line_length = strlen(line) + 1;

	if (line_length != 69) {
		return -1;
	}

	for (int32_t i=0; i < line_length; i++) {
		int8_t c = line[i];

		if ((c >= 48) && (c <= 57)) {
			c -= 48;
		} else if (c == '-') {
			c = 1;
		} else {
			c = 0;
		}

		checksum += (int32_t)c;
	}

	checksum %= modulo;

	return checksum;
}

void PrintTle(TLE TLE_OBJECT) {
	float OrbPeriod = OrbitalPeriod(TLE_OBJECT.MeanMotion);
	uint64_t SMA = SemiMajorAxis(OrbPeriod);

	uint64_t Ap = Apogee(TLE_OBJECT.Eccentricity, SMA);
	uint64_t Pe = Perigee(TLE_OBJECT.Eccentricity, SMA);

	double Epoch_E = NewtonRaphson(TLE_OBJECT.MeanAnomaly*RADS2DEGS, TLE_OBJECT.Eccentricity, *KeplerEquation, *KeplerPrime, TLE_OBJECT.MeanAnomaly*RADS2DEGS, 1E-6, 0xffffffffffffffffULL);

	uint64_t Epoch_R = OrbAlt(TLE_OBJECT.Eccentricity, SMA, Epoch_E);
	uint64_t Epoch_Alt = Epoch_R-(uint64_t)EARTH_RADIUS;

	double Speed_Ap = OrbSpeed(Ap, SMA);
	double Speed_Pe = OrbSpeed(Pe, SMA);
	double Speed_Epoch = OrbSpeed(Epoch_R, SMA);

	double Lorentz_Ap = Lorentz(Speed_Ap);
	double Lorentz_Pe = Lorentz(Speed_Pe);

	time_t current_time = time(NULL);

	struct tm* utc = gmtime(&current_time);

	int current_year = utc->tm_year + 1900;
	double current_day = (double)utc->tm_yday + (double)(utc->tm_hour)/24.0 + (double)(utc->tm_min)/1440.0 + (double)(utc->tm_sec)/86400.0 + 1.0;

	int epoch_year = TLE_OBJECT.EPOCH_YR;

	if (TLE_OBJECT.EPOCH_YR < 57) {
		epoch_year += 2000;
	} else {
		epoch_year += 1900;
	}

	double DeltaTime = (((double)(current_year - epoch_year) * 365.25) + (double)(current_day - TLE_OBJECT.EPOCH)) * 86400.0;

	double Current_MA = TLE_OBJECT.MeanAnomaly * DEGS2RADS + (double)MeanMotionPerSecond(TLE_OBJECT.MeanMotion) * DeltaTime;
	Current_MA -= (double)((uint32_t)(Current_MA / (double)(2.0f*M_PI)) * 2.0f * (double)M_PI);
	Current_MA *= RADS2DEGS;

	double Current_E = NewtonRaphson(Current_MA, TLE_OBJECT.Eccentricity, *KeplerEquation, *KeplerPrime, Current_MA, 1E-8, 0xffffffffffffffffULL);

	uint64_t Current_R = OrbAlt(TLE_OBJECT.Eccentricity, SMA, Current_E);
	uint64_t Current_Alt = Current_R - (uint64_t)EARTH_RADIUS;
	double Current_Spd = OrbSpeed(Current_R, SMA);

	printf("Object name : %s \n", TLE_OBJECT.name);

	printf("---------------------------------- TLE ----------------------------------\n");

	printf("NORAD ID : %0*u%c\n", 5, TLE_OBJECT.NORAD_ID, TLE_OBJECT.Classification);
	printf("COSPAR : %d %0*hu %s\n", TLE_OBJECT.COSPAR_YR, 3, TLE_OBJECT.COSPAR_LN, TLE_OBJECT.COSPAR_OBJ_ID);
	printf("EPOCH : YEAR=20%d DAY=%.8lf\n", TLE_OBJECT.EPOCH_YR, TLE_OBJECT.EPOCH);
	printf("TLE AGE : %s\n", secstohms(DeltaTime));
	printf("(MEAN MOTION)' = %.8lf\n", TLE_OBJECT.FIRST_DERIV_MEAN_MOTION);
	printf("(MEAN MOTION)'' = %.5lf\n", TLE_OBJECT.SECOND_DERIV_MEAN_MOTION);
	printf("B* = %.5e\n", TLE_OBJECT.B_STAR);

	printf("\n");

	printf("INCLINATION : %.4f degs\n", TLE_OBJECT.Inclination);
	printf("LONGITUDE OF ASC. NODE : %.4f degs\n", TLE_OBJECT.AscNodeLong);
	printf("ECCENTRICITY : %.7f\n", TLE_OBJECT.Eccentricity);
	printf("ARG. OF PERIGEE : %.4f degs\n", TLE_OBJECT.PeriArgument);
	printf("MEAN ANOMALY : %.4f degs\n", TLE_OBJECT.MeanAnomaly);
	printf("MEAN MOTION : %.8lf rev/(sid. day)\n", TLE_OBJECT.MeanMotion);

	printf("-------------------------------- RESULTS --------------------------------\n");

	printf("Orbital Period : %.4f secs (%s)\n", OrbPeriod, secstohms(OrbPeriod));
	printf("Semi Major Axis : %llu m\n", SMA);
	printf("Apogee : %llu m | Perigee : %llu m | Epoch : %llu m\n", Ap-(uint64_t)EARTH_RADIUS, Pe-(uint64_t)EARTH_RADIUS, Epoch_Alt);
	printf("Speed @ Ap : %.4lf m/s | Pe : %.4lf m/s | Ep : %.4lf m/s \n", Speed_Ap, Speed_Pe, Speed_Epoch);

	printf("------------------------------- CURRENTLY -------------------------------\n");

	printf("Current Mean Anomaly : %.4lf degs\n", Current_MA);
	printf("Current Alt : %llu m\n", Current_Alt);
	printf("Current Speed : %.4lf m/s\n", Current_Spd);
}

int32_t main() {

	system("clear");

	const bool ReadingFile = false;

	string* filename = "TLEs/stations.tle";
	const uint32_t lookingFor = 25544;

	if (!ReadingFile) {

		TLE* AllObjs = GetAllTle(filename);
		int32_t block_quant = GetTLENumber(filename);

		printf("%d entries loaded\nLooping to find %u\n\n", block_quant, lookingFor);

		TLE CurrentEntry;
		bool found = false;

		for (int32_t i=0; i<block_quant; i++) {
			CurrentEntry = AllObjs[i];
			if (CurrentEntry.NORAD_ID == lookingFor) {
				found = true;
				break;
			}
		}

		if (!found) {
			printf("Unable to find %u amongst the TLEs\n", lookingFor);
			exit(-1);
		}

		while (true) {
			system("clear");
			PrintTle(CurrentEntry);
			Sleep(250);
		}

	} else {
		printf("Opening the files\n");
		int32_t block_number = GetTLENumber(filename);
		// printf("TLE File size : %d blocks\n", block_number);

		time_t rawtime_start;
		time_t rawtime_end;

		time(&rawtime_start);

		TLE* AllObjs = GetAllTle(filename);
		
		time(&rawtime_end);

		printf("Memory usage to store %d blocks : %llu bytes\nTime : %lld\n", block_number, sizeof(TLE) * block_number, rawtime_end-rawtime_start);
		//ExportToStructFile(AllObjs, block_number, "merged.tle_struct");
	}

	return 0;
}
