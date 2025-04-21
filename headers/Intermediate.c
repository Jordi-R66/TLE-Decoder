#include "Intermediate.h"
#include "TimeFuncs.h"

double EccentricAnomalyTolerance = 1E-5 * DEGS2RADS;

time_t current_time;

bool uvInit = false; // Has the unit vector been initialized?
Vector uv; // Unit vector

OrbitData computeOrbitData(TLE* Object) {
	OrbitData output;

	memset(&output, 0, ORBIT_DATA_LENGTH);

	output.OrbPeriod = OrbitalPeriod(Object->MeanMotion);
	output.SMA = SemiMajorAxis(output.OrbPeriod);
	output.n = AngularSpeed(output.SMA);

	output.Ap = Apoapsis(Object->Eccentricity, output.SMA);
	output.Pe = Periapsis(Object->Eccentricity, output.SMA);

	output.longPeri = longitudeOfPeriapsis(Object->AscNodeLong, Object->PeriArg);

	output.longPeri = fmod(output.longPeri, 360.0);
	output.longPeri *= DEGS2RADS;

	uint16_t epoch_year = Object->EPOCH_YR;

	if (epoch_year < 57) {
		epoch_year += 2000;
	} else {
		epoch_year += 1900;
	}

	output.epoch = tleEpochToJJ(epoch_year, Object->EPOCH);

	return output;
}

EpochData computeEpochData(TLE* Object, OrbitData* orbitData, bool realTime) {
	EpochData output;

	double deltaTime = 0.0;

	if (realTime) {
		deltaTime = (CurrentEpoch() - orbitData->epoch) * EARTH_DAY_LENGTH;
	}

	memset(&output, 0, EPOCH_DATA_LENGTH);

	double MA = Object->MeanAnomaly + (orbitData->n * deltaTime * RADS2DEGS);

	double E_Approx = MA + Object->Eccentricity * sin(MA * DEGS2RADS);
	double E = NewtonRaphson(MA, Object->Eccentricity, *KeplerEquation, *KeplerPrime, E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
	double TA = TrueAnomaly(Object->Eccentricity, E);

	output.MA = MA;
	output.E = E;
	output.TA = TA;

	double R = OrbAltTA(Object->Eccentricity, orbitData->SMA, TA);
	double Alt = R - EARTH_RADIUS;
	double Spd = OrbSpeed(R, orbitData->SMA);

	output.R = R;
	output.Alt = Alt;
	output.Spd = Spd;

	// Now computing the unit vector - if not initialized

	if (!uvInit) {
		double AscNodeTA = longitudeToTA(Object->AscNodeLong * DEGS2RADS, orbitData->longPeri);
		double AscNodeR = OrbAltTA(Object->Eccentricity, orbitData->SMA, AscNodeTA);

		Vector AscNode = coordsFromTA(AscNodeR, AscNodeTA);
		Vector rotAN = Rotate2D(&AscNode, orbitData->longPeri);
		KeplerCoords2D_t AscNode2D = *(KeplerCoords2D_t*)rotAN.data;

		uv = unitVector2D(AscNode2D.x, AscNode2D.y);

		deallocVector(&AscNode);
		deallocVector(&rotAN);

		uvInit = true;
	}

	// Now computing 2D

	Vector coords2D = coordsFromTA(R, TA);
	Vector rot2D = Rotate2D(&coords2D, orbitData->longPeri);

	output.coords2D = *(KeplerCoords2D_t*)rot2D.data;

	deallocVector(&coords2D);

	// Now Computing 3D

	

	return output;
}

void PrintTle(TLE* Object) {
	OrbitData orbitData = computeOrbitData(Object);
	string* periodString = secstohms(orbitData.OrbPeriod);

	double rotAngle = orbitData.longPeri;

	double AscNodeTA = longitudeToTA(Object->AscNodeLong * DEGS2RADS, orbitData.longPeri);
	double AscNodeR = OrbAltTA(Object->Eccentricity, orbitData.SMA, AscNodeTA);
	Vector AscNode = coordsFromTA(AscNodeR, AscNodeTA);

	Vector rotAN = Rotate2D(&AscNode, rotAngle);

	KeplerCoords2D_t AscNode2D = *(KeplerCoords2D_t*)rotAN.data;

	Vector unitVector = unitVector2D(AscNode2D.x, AscNode2D.y);

	deallocVector(&AscNode);
	deallocVector(&rotAN);

	double Epoch_E_Approx = (Object->MeanAnomaly * DEGS2RADS) + Object->Eccentricity * sin(Object->MeanAnomaly * DEGS2RADS);
	double Epoch_E = NewtonRaphson(Object->MeanAnomaly * DEGS2RADS, Object->Eccentricity, *KeplerEquation, *KeplerPrime, Epoch_E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
	double Epoch_TA = TrueAnomaly(Object->Eccentricity, Epoch_E);

	double Epoch_R = OrbAltTA(Object->Eccentricity, orbitData.SMA, Epoch_TA);
	double Epoch_Alt = Epoch_R - (double)EARTH_RADIUS;

	double Speed_Ap = OrbSpeed(orbitData.Ap, orbitData.SMA);
	double Speed_Pe = OrbSpeed(orbitData.Pe, orbitData.SMA);
	double Speed_Epoch = OrbSpeed(Epoch_R, orbitData.SMA);

	current_time = time(NULL);

	struct tm* utc = gmtime(&current_time);

	int32_t current_year = utc->tm_year + 1900;
	double current_day = (double)utc->tm_yday + (double)(utc->tm_hour) / 24.0 + (double)(utc->tm_min) / 1440.0 + (double)(utc->tm_sec) / EARTH_DAY_LENGTH + 1.0;

	int32_t epoch_year = Object->EPOCH_YR;

	if (epoch_year < 57) {
		epoch_year += 2000;
	} else {
		epoch_year += 1900;
	}

	double DeltaTime = (((double)(current_year - epoch_year) * CALENDAR_YEAR) + (double)(current_day - Object->EPOCH)) * EARTH_DAY_LENGTH;

	double Current_MA = (Object->MeanAnomaly) + (orbitData.n * DeltaTime * RADS2DEGS);

	double Current_E_Approx = Current_MA + Object->Eccentricity * sin(Current_MA * DEGS2RADS);
	double Current_E = NewtonRaphson(Current_MA, Object->Eccentricity, *KeplerEquation, *KeplerPrime, Current_E_Approx, EccentricAnomalyTolerance, DEFAULT_ITER);
	double Current_TA = TrueAnomaly(Object->Eccentricity, Current_E);

	double Current_R = OrbAltTA(Object->Eccentricity, orbitData.SMA, Current_TA);
	double Current_Alt = Current_R - (double)EARTH_RADIUS;
	double Current_Spd = OrbSpeed(Current_R, orbitData.SMA);

	Current_MA = fmod(Current_MA, 360.0);

	Current_TA *= RADS2DEGS;
	Current_TA = fmod(Current_TA, 360.0);
	Current_TA *= DEGS2RADS;

	Vector current2D = coordsFromTA(Current_R, Current_TA);
	Vector rotCurrent2D = Rotate2D(&current2D, rotAngle);
	Vector current3D = Rotate3D(&unitVector, &rotCurrent2D, Object->Inclination * DEGS2RADS);

	KeplerCoords3D_t coords3D = *(KeplerCoords3D_t*)current3D.data;

	deallocVector(&current2D);
	deallocVector(&rotCurrent2D);
	deallocVector(&current3D);

	double altitude3D = sqrt(pow(coords3D.x, 2) + pow(coords3D.y, 2) + pow(coords3D.z, 2)) - EARTH_RADIUS;

	printf("Object name : %s\n", Object->name);

	printf("---------------------------------- TLE ----------------------------------\n");

	printf("NORAD ID : %0*u%c\n", 5, Object->NORAD_ID, Object->Classification);
	printf("COSPAR : %0*d %0*hu %s\n", 2, Object->COSPAR_YR, 3, Object->COSPAR_LN, Object->COSPAR_ID);
	printf("EPOCH : YEAR=20%d DAY=%.8lf\n", Object->EPOCH_YR, Object->EPOCH);
	printf("TLE AGE : %s\n", periodString);
	printf("(MEAN MOTION)' = %.8lf\n", Object->FIRST_DERIV_MEAN_MOTION);
	printf("(MEAN MOTION)'' = %.5lf\n", Object->SECOND_DERIV_MEAN_MOTION);
	printf("B* = %.5e\n", Object->B_STAR);

	printf("\n");

	printf("INCLINATION : %.4lf degs\n", Object->Inclination);
	printf("LONGITUDE OF ASC. NODE : %.4f degs\n", Object->AscNodeLong);
	printf("LONGITUDE OF PERIAPSIS : %.4f degs\n", orbitData.longPeri * RADS2DEGS);
	printf("ECCENTRICITY : %.7lf\n", Object->Eccentricity);
	printf("ARG. OF PERIAPSIS : %.4lf degs\n", Object->PeriArg);
	printf("MEAN ANOMALY : %.4lf degs\n", Object->MeanAnomaly);
	printf("MEAN MOTION : %.8lf rev/(sid. day)\n", Object->MeanMotion);

	printf("-------------------------------- RESULTS --------------------------------\n");

	printf("Orbital Period : %.4lf secs (%s)\n", orbitData.OrbPeriod, secstohms(orbitData.OrbPeriod));
	printf("Semi Major Axis : %.0lf m\n", orbitData.SMA);
	printf("Apoapsis : %.0lf m | Periapsis : %.0lf m | Epoch : %.0lf m\n", orbitData.Ap - (double)EARTH_RADIUS, orbitData.Pe - (double)EARTH_RADIUS, Epoch_Alt);
	printf("Speed @ Ap : %.4lf m/s | Pe : %.4lf m/s | Ep : %.4lf m/s \n", Speed_Ap, Speed_Pe, Speed_Epoch);

	printf("------------------------------- CURRENTLY -------------------------------\n");

	printf("DATE (UTC) : %0*d/%0*d/%0*d %0*d:%0*d:%0*d\n", 2, utc->tm_mday, 2, utc->tm_mon + 1, 4, epoch_year, 2, utc->tm_hour, 2, utc->tm_min, 2, utc->tm_sec);
	printf("X Coord : %.0lf m\n", coords3D.x);
	printf("Y Coord : %.0lf m\n", coords3D.y);
	printf("Z Coord : %.0lf m\n", coords3D.z);
	printf("ALTITUDE : %.0lf m\n", Current_Alt);
	//printf("ALTITUDE 2D : %.0lf m\n", altitude2D);
	printf("ALTITUDE 3D : %.0lf m\n", altitude3D);
	printf("SPEED : %.2lf m/s\n", Current_Spd);

	printf("-------------------------------------------------------------------------\n");

	deallocVector(&unitVector);
}