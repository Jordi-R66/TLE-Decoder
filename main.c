#include "libs/TleParser.h"
#include "libs/OrbMaths.h"
#include "libs/TleFiles.h"
#include "libs/TermFuncs.h"
#include "libs/Algos.h"
#include "libs/Kepler.h"
#include "libs/Ellipses.h"

#include "libs/Common.h"

//#include "libs/logger.h"

#define CHECKSUM_MODULO 10
#define DEFAULT_ITER 100000

#define CALENDAR_YEAR 365.25
//#define DEBUG_MODE

string* filename = "TLEs/active.tle";
uint32_t lookingFor = 23802;

double EccentricAnomalyTolerance = 1E-5 * DEGS2RADS;

//bool debugMode = false;
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

	/*
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
	*/

	double OrbPeriod = OrbitalPeriod(Object.MeanMotion);
	double SMA = SemiMajorAxis(OrbPeriod);
	double n = AngularSpeed(SMA);

	double Ap = Apoapsis(Object.Eccentricity, SMA);
	double Pe = Periapsis(Object.Eccentricity, SMA);

	double longPeri = (Object.AscNodeLong + Object.PeriArg) * DEGS2RADS;

	longPeri *= RADS2DEGS;
	longPeri -= (double)((uint32_t)(longPeri / 360.0) * 360);
	longPeri *= DEGS2RADS;

	double AscNodeTA = Object.AscNodeLong * DEGS2RADS - longPeri;
	double AscNodeR = OrbAltTA(Object.Eccentricity, SMA, AscNodeTA);

	KeplerCoords2D_t bari = baricenterRelativeToFocal(SMA, Object.Eccentricity);
	KeplerCoords2D_t focal = FocalRelativeToBaricenter(SMA, Object.Eccentricity);
	KeplerCoords2D_t AscNode = coordsFromTA(AscNodeR, AscNodeTA);

	changeReferential2D(AscNode, bari, &AscNode);

	KeplerCoords3D_t focal3D = Rotate3DCoordsAroundAxis(AscNode, focal, Object.Inclination * DEGS2RADS);
	KeplerCoords3D_t AscNode3D = Rotate3DCoordsAroundAxis(AscNode, AscNode, Object.Inclination * DEGS2RADS);

	printf("X : %lf | Y : %lf | Z : %lf\n", AscNode3D.x, AscNode3D.y, AscNode3D.z);

	double Epoch_E_Approx = (Object.MeanAnomaly * DEGS2RADS) + Object.Eccentricity * sin(Object.MeanAnomaly * DEGS2RADS);
	double Epoch_E = NewtonRaphson(Object.MeanAnomaly * DEGS2RADS, Object.Eccentricity, *KeplerEquation, *KeplerPrime, Epoch_E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
	double Epoch_TA = TrueAnomaly(Object.Eccentricity, Epoch_E);

	double Epoch_R = OrbAltTA(Object.Eccentricity, SMA, Epoch_TA);
	double Epoch_Alt = Epoch_R - (double)EARTH_RADIUS;

	double Speed_Ap = OrbSpeed(Ap, SMA);
	double Speed_Pe = OrbSpeed(Pe, SMA);
	double Speed_Epoch = OrbSpeed(Epoch_R, SMA);

	current_time = time(NULL);

	struct tm* utc = gmtime(&current_time);

	int32_t current_year = utc->tm_year + 1900;
	double current_day = (double)utc->tm_yday + (double)(utc->tm_hour) / 24.0 + (double)(utc->tm_min) / 1440.0 + (double)(utc->tm_sec) / EARTH_DAY_LENGTH + 1.0;

	int32_t epoch_year = Object.EPOCH_YR;

	if (epoch_year < 57) {
		epoch_year += 2000;
	}
	else {
		epoch_year += 1900;
	}

	//double DeltaTime;
#ifdef DEBUG_MODE
	uint64_t orb_period = ((uint64_t)OrbitalPeriod(Object.MeanMotion) + 2);
	FILE* fp = fopen("log_23802.csv", "w");

	if (fp == NULL) {
		fprintf(stderr, "Error while opening the file\n");
		exit(EXIT_FAILURE);
		return;
	}

	fprintf(fp, "Time,altitude via 2D coords,altitude via 3D coords,altitude via ecc ano,altitude via true ano\n");

	for (uint64_t DeltaTime = 0; DeltaTime < orb_period; DeltaTime++) {
		double Current_MA = (Object.MeanAnomaly * DEGS2RADS) + (n * DeltaTime);

		double Current_E_Approx = Current_MA + Object.Eccentricity * sin(Current_MA);
		double Current_E = NewtonRaphson(Current_MA, Object.Eccentricity, *KeplerEquation, *KeplerPrime, Current_E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
		double Current_TA = TrueAnomaly(Object.Eccentricity, Current_E);

		double Current_R = OrbAltTA(Object.Eccentricity, SMA, Current_TA);
		double Current_Alt = Current_R - (double)EARTH_RADIUS;
		double Current_Spd = OrbSpeed(Current_R, SMA);

		Current_MA *= RADS2DEGS;
		Current_MA -= (double)((uint32_t)(Current_MA / 360.0) * 360);
		Current_MA *= DEGS2RADS;

		Current_TA *= RADS2DEGS;
		Current_TA -= (double)((uint32_t)(Current_TA / 360.0) * 360);
		Current_TA *= DEGS2RADS;

		KeplerCoords2D_t coords_2d = coordsFromTA(Current_R, Current_TA * DEGS2RADS);//basic2DKeplerCoords(SMA, Object.Eccentricity, Current_E);
		KeplerCoords2D_t absCoords;

		changeReferential2D(coords_2d, bari, &absCoords);

		double altitude2D = sqrt(pow(coords_2d.x, 2) + pow(coords_2d.y, 2)) - EARTH_RADIUS;

		KeplerCoords3D_t absCoords3D = Rotate3DCoordsAroundAxis(AscNode, absCoords, Object.Inclination * DEGS2RADS);
		KeplerCoords3D_t coords3D;

		changeReferential3D(absCoords3D, focal3D, &coords3D);

		//double altitude = sqrt(pow(coords_2d.x, 2) + pow(coords_2d.y, 2)) - EARTH_RADIUS;
		double altitude3D = sqrt(pow(coords3D.x, 2) + pow(coords3D.y, 2) + pow(coords3D.z, 2)) - EARTH_RADIUS;

		double coords2dAlt = altitude2D;
		double coords3dAlt = altitude3D;
		double keplerAlt = keplerDistance(SMA, Object.Eccentricity, Current_E) - (double)EARTH_RADIUS;
		double trueAnoAlt = OrbAltTA(Object.Eccentricity, SMA, Current_TA) - (double)EARTH_RADIUS;

		fprintf(fp, "%llu,%lf,%lf,%lf,%lf\n", DeltaTime, coords2dAlt, coords3dAlt, keplerAlt, trueAnoAlt);

		//addRecord(&file, record);
	}

	fclose(fp);
#else
	double DeltaTime = (((double)(current_year - epoch_year) * CALENDAR_YEAR) + (double)(current_day - Object.EPOCH)) * EARTH_DAY_LENGTH;

	double Current_MA = (Object.MeanAnomaly * DEGS2RADS) + (n * DeltaTime);

	double Current_E_Approx = Current_MA + Object.Eccentricity * sin(Current_MA);
	double Current_E = NewtonRaphson(Current_MA, Object.Eccentricity, *KeplerEquation, *KeplerPrime, Current_E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
	double Current_TA = TrueAnomaly(Object.Eccentricity, Current_E);

	double Current_R = OrbAltTA(Object.Eccentricity, SMA, Current_TA);
	double Current_Alt = Current_R - (double)EARTH_RADIUS;
	double Current_Spd = OrbSpeed(Current_R, SMA);

	Current_MA *= RADS2DEGS;
	Current_MA -= (double)((uint32_t)(Current_MA / 360.0) * 360);

	Current_TA *= RADS2DEGS;
	Current_TA -= (double)((uint32_t)(Current_TA / 360.0) * 360);

	KeplerCoords2D_t coords_2d = coordsFromTA(Current_R, Current_TA * DEGS2RADS);//basic2DKeplerCoords(SMA, Object.Eccentricity, Current_E);
	KeplerCoords2D_t absCoords;

	changeReferential2D(coords_2d, bari, &absCoords);

	double altitude2D = sqrt(pow(coords_2d.x, 2) + pow(coords_2d.y, 2)) - EARTH_RADIUS;

	KeplerCoords3D_t absCoords3D = Rotate3DCoordsAroundAxis(AscNode, absCoords, Object.Inclination * DEGS2RADS);
	KeplerCoords3D_t coords3D;

	changeReferential3D(absCoords3D, focal3D, &coords3D);

	//double altitude = sqrt(pow(coords_2d.x, 2) + pow(coords_2d.y, 2)) - EARTH_RADIUS;
	double altitude3D = sqrt(pow(coords3D.x, 2) + pow(coords3D.y, 2) + pow(coords3D.z, 2)) - EARTH_RADIUS;

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
	printf("LONGITUDE OF PERIAPSIS : %.4f degs\n", longPeri * RADS2DEGS);
	printf("ECCENTRICITY : %.7lf\n", Object.Eccentricity);
	printf("ARG. OF PERIAPSIS : %.4lf degs\n", Object.PeriArg);
	printf("MEAN ANOMALY : %.4lf degs\n", Object.MeanAnomaly);
	printf("MEAN MOTION : %.8lf rev/(sid. day)\n", Object.MeanMotion);

	printf("-------------------------------- RESULTS --------------------------------\n");

	printf("Orbital Period : %.4lf secs (%s)\n", OrbPeriod, secstohms(OrbPeriod));
	printf("Semi Major Axis : %.0lf m\n", SMA);
	printf("Apoapsis : %.0lf m | Periapsis : %.0lf m | Epoch : %.0lf m\n", Ap - (double)EARTH_RADIUS, Pe - (double)EARTH_RADIUS, Epoch_Alt);
	printf("Speed @ Ap : %.4lf m/s | Pe : %.4lf m/s | Ep : %.4lf m/s \n", Speed_Ap, Speed_Pe, Speed_Epoch);

	printf("------------------------------- CURRENTLY -------------------------------\n");

	printf("DATE (UTC) : %0*d/%0*d/%0*d %0*d:%0*d:%0*d\n", 2, utc->tm_mday, 2, utc->tm_mon + 1, 4, epoch_year, 2, utc->tm_hour, 2, utc->tm_min, 2, utc->tm_sec);
	printf("X Coord : %.0lf m\tX Earth : %.0lf m\n", coords3D.x, focal3D.x);
	printf("Y Coord : %.0lf m\tY Earth : %.0lf m\n", coords3D.y, focal3D.y);
	printf("Z Coord : %.0lf m\tZ Earth : %.0lf m\n", coords3D.z, focal3D.z);
	printf("ALTITUDE : %.0lf m\n", Current_Alt);
	printf("ALTITUDE 2D : %.0lf m\n", altitude2D);
	printf("ALTITUDE 3D : %.0lf m\n", altitude3D);
	printf("SPEED : %.2lf m/s\n", Current_Spd);

	printf("-------------------------------------------------------------------------\n");
#endif

	//writeFile(&file);

	//for (size_t i = 0; i < file.n_records; i++) {
		//freeRecord(file.records[i]);
	//}
	//freeFile(&file);
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
			#ifndef DEBUG_MODE
				sleep(1);
			#else
				break;
			#endif
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

		printf("Memory usage to store %d blocks : %llu bytes\nTime : %lld\n", block_number, sizeof(TLE) * block_number, rawtime_end - rawtime_start);
		//ExportToStructFile(AllObjs, block_number, "merged.tle_struct");
	}

	return 0;
}
