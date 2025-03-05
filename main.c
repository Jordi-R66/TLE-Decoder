#include "libs/TleParser.h"
#include "libs/OrbMaths.h"
#include "libs/TleFiles.h"
#include "libs/TermFuncs.h"
#include "libs/Algos.h"
#include "libs/Referential.h"

#include "libs/Common.h"

#define CHECKSUM_MODULO 10
#define DEFAULT_ITER 100000

string* filename = "TLEs/stations.tle";
uint32_t lookingFor = 25544;

double EccentricAnomalyTolerance = 0.0001 * DEGS2RADS;

time_t current_time;

/* void InterpretArgs(uint8_t n, char** args) {
	if (n == 5) {
		for (uint8_t i=0; i<n; i++) {
			printf("%s\n", args[i]);
			if (!strcmp(args[i], "-s")) {
				strcpy(filename, args[i+1]);
			} else if (!strcmp(args[i], "-u")) {
				lookingFor = strint(args[i+1]);
			}
		}

		printf("%s\n%u\n\n", filename, lookingFor);
	} else {
		exit(-1);
	}
}*/

void PrintTle(TLE Object) {

	double Coord3D_X, Coord3D_Y, Coord3D_Z;
	double Speed3D_X, Speed3D_Y, Speed3D_Z;

	Matrix ArgPeriRot;
	ArgPeriRot.cols = 3;
	ArgPeriRot.rows = 3;
	allocMatrix(&ArgPeriRot);

	Matrix IncliRot;
	IncliRot.cols = 3;
	IncliRot.rows = 3;
	allocMatrix(&IncliRot);

	Matrix ANRot;
	ANRot.cols = 3;
	ANRot.rows = 3;
	allocMatrix(&ANRot);

	Coords2D OrbCoords2D;
	OrbCoords2D.cols = 1;
	OrbCoords2D.rows = 2;
	allocMatrix(&OrbCoords2D);

	Coords3D RefCoords3D;
	RefCoords3D.cols = 1;
	RefCoords3D.rows = 3;
	allocMatrix(&RefCoords3D);

	Vector2D OrbSpeed2D;
	OrbSpeed2D.cols = 1;
	OrbSpeed2D.rows = 2;
	allocMatrix(&OrbSpeed2D);

	Vector3D RefSpeed3D;
	RefSpeed3D.cols = 1;
	RefSpeed3D.rows = 3;
	allocMatrix(&RefSpeed3D);

	GenRotMatrices(&ArgPeriRot, &IncliRot, &ANRot, Object.PeriArg * DEGS2RADS, Object.Inclination * DEGS2RADS, Object.AscNodeLong * DEGS2RADS);

	double OrbPeriod = OrbitalPeriod(Object.MeanMotion);
	uint64_t SMA = SemiMajorAxis(OrbPeriod);
	double n = AngularSpeed(SMA);

	uint64_t Ap = Apoapsis(Object.Eccentricity, SMA);
	uint64_t Pe = Periapsis(Object.Eccentricity, SMA);

	double Epoch_E_Approx = (Object.MeanAnomaly * DEGS2RADS) + Object.Eccentricity * sin(Object.MeanAnomaly * DEGS2RADS);
	double Epoch_E = NewtonRaphson(Object.MeanAnomaly * DEGS2RADS, Object.Eccentricity, *KeplerEquation, *KeplerPrime, Epoch_E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
	double Epoch_TA = TrueAnomaly(Object.Eccentricity, Epoch_E);

	uint64_t Epoch_R = OrbAltTA(Object.Eccentricity, SMA, Epoch_TA);
	uint64_t Epoch_Alt = Epoch_R - (uint64_t)EARTH_RADIUS;

	double Speed_Ap = OrbSpeed(Ap, SMA);
	double Speed_Pe = OrbSpeed(Pe, SMA);
	double Speed_Epoch = OrbSpeed(Epoch_R, SMA);

	current_time = time(NULL);

	struct tm* utc = gmtime(&current_time);

	int32_t current_year = utc->tm_year + 1900;
	double current_day = (double)utc->tm_yday + (double)(utc->tm_hour) / 24.0 + (double)(utc->tm_min) / 1440.0 + (double)(utc->tm_sec) / 86400.0 + 1.0;

	int32_t epoch_year = Object.EPOCH_YR;

	if (epoch_year < 57) {
		epoch_year += 2000;
	}
	else {
		epoch_year += 1900;
	}

	double DeltaTime = (((double)(current_year - epoch_year) * 365.25) + (double)(current_day - Object.EPOCH)) * 86400.0;

	double Current_MA = (Object.MeanAnomaly * DEGS2RADS) + (n * DeltaTime);

	double Current_E_Approx = Current_MA + Object.Eccentricity * sin(Current_MA);
	double Current_E = NewtonRaphson(Current_MA, Object.Eccentricity, *KeplerEquation, *KeplerPrime, Current_E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
	double Current_TA = TrueAnomaly(Object.Eccentricity, Current_E);

	uint64_t Current_R = OrbAltTA(Object.Eccentricity, SMA, Current_TA);
	uint64_t Current_Alt = Current_R - (uint64_t)EARTH_RADIUS;
	double Current_Spd = OrbSpeed(Current_R, SMA);

	Compute2DCoords(&OrbCoords2D, Current_R, Current_TA);
	Compute2DSpeedVector(&OrbSpeed2D, SMA, Object.Eccentricity, Current_TA);

	RotateCoords(&ArgPeriRot, &IncliRot, &ANRot, &OrbCoords2D, &RefCoords3D);
	RotateVector(&ArgPeriRot, &IncliRot, &ANRot, &OrbSpeed2D, &RefSpeed3D);

	Coord3D_X = RefCoords3D.data[0];
	Coord3D_Y = RefCoords3D.data[1];
	Coord3D_Z = RefCoords3D.data[2];

	Speed3D_Z = RefSpeed3D.data[0];
	Speed3D_X = RefSpeed3D.data[1];
	Speed3D_Y = RefSpeed3D.data[2];

	Current_MA *= RADS2DEGS;
	Current_MA -= (double)((uint32_t)(Current_MA / 360.0) * 360);

	Current_TA *= RADS2DEGS;
	Current_TA -= (double)((uint32_t)(Current_TA / 360.0) * 360);

	printf("Object name : %s\n", Object.name);

	printf("---------------------------------- TLE ----------------------------------\n");

	printf("NORAD ID : %0*u%c\n", 5, Object.NORAD_ID, Object.Classification);
	printf("COSPAR : %0*d %0*hu %s\n", 2, Object.COSPAR_YR, 3, Object.COSPAR_LN, Object.COSPAR_ID);
	printf("EPOCH : YEAR=20%d DAY=%.8lf\n", Object.EPOCH_YR, Object.EPOCH);
	printf("TLE AGE : %s\n", secstohms(DeltaTime));
	printf("(MEAN MOTION)' = %.8lf\n", Object.FIRST_DERIV_MEAN_MOTION);
	printf("(MEAN MOTION)'' = %.5lf\n", Object.SECOND_DERIV_MEAN_MOTION);
	printf("B* = %.5e\n", Object.B_STAR);

	printf("\n");

	printf("INCLINATION : %.4lf degs\n", Object.Inclination);
	printf("LONGITUDE OF ASC. NODE : %.4f degs\n", Object.AscNodeLong);
	printf("ECCENTRICITY : %.7lf\n", Object.Eccentricity);
	printf("ARG. OF PERIAPSIS : %.4lf degs\n", Object.PeriArg);
	printf("MEAN ANOMALY : %.4lf degs\n", Object.MeanAnomaly);
	printf("MEAN MOTION : %.8lf rev/(sid. day)\n", Object.MeanMotion);

	printf("-------------------------------- RESULTS --------------------------------\n");

	printf("Orbital Period : %.4lf secs (%s)\n", OrbPeriod, secstohms(OrbPeriod));
	printf("Semi Major Axis : %llu m\n", SMA);
	printf("Apoapsis : %llu m | Periapsis : %llu m | Epoch : %llu m\n", Ap - (uint64_t)EARTH_RADIUS, Pe - (uint64_t)EARTH_RADIUS, Epoch_Alt);
	printf("Speed @ Ap : %.4lf m/s | Pe : %.4lf m/s | Ep : %.4lf m/s \n", Speed_Ap, Speed_Pe, Speed_Epoch);

	printf("------------------------------- CURRENTLY -------------------------------\n");

	printf("DATE (UTC) : %0*d/%0*d/%0*d %0*d:%0*d:%0*d\n", 2, utc->tm_mday, 2, utc->tm_mon + 1, 4, epoch_year, 2, utc->tm_hour, 2, utc->tm_min, 2, utc->tm_sec);
	// printf("MEAN ANOMALY : %.4lf degs\n", Current_MA);
	// printf("ECC. ANOMALY : %.4lf rads\n", Current_E);
	// printf("TRUE ANOMALY : %.4lf degs\n", Current_TA);
	printf("X Coord : %.2lf m\tX Speed : %.2lf m/s\n", Coord3D_X, Speed3D_X);
	printf("Y Coord : %.2lf m\tY Speed : %.2lf m/s\n", Coord3D_Y, Speed3D_Y);
	printf("Z Coord : %.2lf m\tZ Speed : %.2lf m/s\n", Coord3D_Z, Speed3D_Z);
	printf("ALTITUDE : %llu m\n", Current_Alt);
	printf("SPEED : %.4lf m/s\n", Current_Spd);

	deallocMatrix(&ArgPeriRot);
	deallocMatrix(&IncliRot);
	deallocMatrix(&ANRot);

	deallocMatrix(&OrbCoords2D);
	deallocMatrix(&RefCoords3D);

	deallocMatrix(&OrbSpeed2D);
	deallocMatrix(&RefSpeed3D);
}

int32_t main(int argc, char* argv[]) {
	// InterpretArgs(argc, argv);

	clear();

	const bool ReadingFile = false;

	if (!ReadingFile) {

		TLE* AllObjs = GetAllTle(filename);
		int32_t block_quant = GetTLENumber(filename);

		printf("%d entries loaded\nLooping to find %u\n\n", block_quant, lookingFor);

		TLE CurrentEntry;
		bool found = false;

		for (int32_t i = 0; i < block_quant; i++) {
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
			sleep(1);
		}

	}
	else {
		printf("Opening the files\n");
		int32_t block_number = GetTLENumber(filename);
		// printf("TLE File size : %d blocks\n", block_number);

		time_t rawtime_start;
		time_t rawtime_end;

		time(&rawtime_start);

		TLE* AllObjs = GetAllTle(filename);

		time(&rawtime_end);

		printf("Memory usage to store %d blocks : %llu bytes\nTime : %lld\n", block_number, sizeof(TLE) * block_number, rawtime_end - rawtime_start);
		//ExportToStructFile(AllObjs, block_number, "merged.tle_struct");
	}

	return 0;
}
