#include <time.h>

#include "libs/TleParser.h"
#include "libs/OrbMaths.h"
#include "libs/TLE_Files.h"
#include "libs/TermFuncs.h"
#include "libs/Algos.h"

#include "libs/Common.h"

#define CHECKSUM_MODULO 10

string* filename = "TLEs/stations.tle";
uint32_t lookingFor = 25544;

float EccentricAnomalyTolerance = 1e-15;

time_t current_time;

// void InterpretArgs(uint8_t n, char** args) {
// 	if (n == 5) {
// 		for (uint8_t i=0; i<n; i++) {
// 			printf("%s\n", args[i]);
// 			if (!strcmp(args[i], "-s")) {
// 				strcpy(filename, args[i+1]);
// 			} else if (!strcmp(args[i], "-u")) {
// 				lookingFor = strint(args[i+1]);
// 			}
// 		}

// 		printf("%s\n%u\n\n", filename, lookingFor);
// 	} else {
// 		exit(-1);
// 	}
// }

void PrintTle(TLE TLE_OBJECT) {
	float OrbPeriod = OrbitalPeriod(TLE_OBJECT.MeanMotion);
	uint64_t SMA = SemiMajorAxis(OrbPeriod);
	double n = AngularSpeed(SMA);

	uint64_t Ap = Apoapsis(TLE_OBJECT.Eccentricity, SMA);
	uint64_t Pe = Periapsis(TLE_OBJECT.Eccentricity, SMA);

	double Epoch_E = NewtonRaphson(TLE_OBJECT.MeanAnomaly*DEGS2RADS, TLE_OBJECT.Eccentricity, *KeplerEquation, *KeplerPrime, TLE_OBJECT.MeanAnomaly*DEGS2RADS, EccentricAnomalyTolerance, 0xffffffffffffffffULL);
	double Epoch_TA = TrueAnomaly(TLE_OBJECT.Eccentricity, Epoch_E);

	uint64_t Epoch_R = OrbAlt(TLE_OBJECT.Eccentricity, SMA, Epoch_E);
	uint64_t Epoch_Alt = Epoch_R-(uint64_t)EARTH_RADIUS;

	double Speed_Ap = OrbSpeed(Ap, SMA);
	double Speed_Pe = OrbSpeed(Pe, SMA);
	double Speed_Epoch = OrbSpeed(Epoch_R, SMA);

	current_time = time(NULL);

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

	double Current_MA = (TLE_OBJECT.MeanAnomaly * DEGS2RADS) + (n * DeltaTime);
	double Current_E = NewtonRaphson(Current_MA, TLE_OBJECT.Eccentricity, *KeplerEquation, *KeplerPrime, Current_MA, EccentricAnomalyTolerance, 0xffffffffffffffffULL);
	double Current_TA = TrueAnomaly(TLE_OBJECT.Eccentricity, Current_E);

	uint64_t Current_R = OrbAltTA(TLE_OBJECT.Eccentricity, SMA, Current_TA);
	uint64_t Current_Alt = Current_R - (uint64_t)EARTH_RADIUS;
	double Current_Spd = OrbSpeed(Current_R, SMA);

	Current_MA *= RADS2DEGS;
	Current_MA -= (double)((uint32_t)(Current_MA / 360.0) * 360);

	Current_TA *= RADS2DEGS;
	Current_TA -= (double)((uint32_t)(Current_TA / 360.0) * 360);

	printf("Object name : %s\n", TLE_OBJECT.name);

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
	printf("ARG. OF PERIAPSIS : %.4f degs\n", TLE_OBJECT.PeriArg);
	printf("MEAN ANOMALY : %.4f degs\n", TLE_OBJECT.MeanAnomaly);
	printf("MEAN MOTION : %.8lf rev/(sid. day)\n", TLE_OBJECT.MeanMotion);

	printf("-------------------------------- RESULTS --------------------------------\n");

	printf("Orbital Period : %.4f secs (%s)\n", OrbPeriod, secstohms(OrbPeriod));
	printf("Semi Major Axis : %llu m\n", SMA);
	printf("Apoapsis : %llu m | Periapsis : %llu m | Epoch : %llu m\n", Ap-(uint64_t)EARTH_RADIUS, Pe-(uint64_t)EARTH_RADIUS, Epoch_Alt);
	printf("Speed @ Ap : %.4lf m/s | Pe : %.4lf m/s | Ep : %.4lf m/s \n", Speed_Ap, Speed_Pe, Speed_Epoch);

	printf("------------------------------- CURRENTLY -------------------------------\n");

	printf("DATE (UTC) : %0*d/%0*d/%0*d %0*d:%0*d:%0*d\n", 2, utc->tm_mday, 2, utc->tm_mon+1, 4, epoch_year, 2, utc->tm_hour, 2, utc->tm_min, 2, utc->tm_sec);
	printf("MEAN ANOMALY : %.4lf degs\n", Current_MA);
	printf("TRUE ANOMALY ; %.4lf degs\n", Current_TA);
	printf("ALTITUDE : %llu m\n", Current_Alt);
	printf("SPEED : %.4lf m/s\n", Current_Spd);
}

int32_t main(uint8_t argc, char *argv[]) {
	// InterpretArgs(argc, argv);

	clear();

	const bool ReadingFile = false;

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
			clear();
			PrintTle(CurrentEntry);
			usleep(1000000/15);
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
