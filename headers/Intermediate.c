#include "Intermediate.h"

double EccentricAnomalyTolerance = 1E-5 * DEGS2RADS;

time_t current_time;

void PrintTle(TLE Object) {

	double OrbPeriod = OrbitalPeriod(Object.MeanMotion);
	double SMA = SemiMajorAxis(OrbPeriod);
	double n = AngularSpeed(SMA);

	double Ap = Apoapsis(Object.Eccentricity, SMA);
	double Pe = Periapsis(Object.Eccentricity, SMA);

	double longPeri = longitudeOfPeriapsis(Object.AscNodeLong, Object.PeriArg);

	longPeri = fmod(longPeri, 360.0);
	longPeri *= DEGS2RADS;

	value_t rotAngle = longPeri;

	double AscNodeTA = longitudeToTA(Object.AscNodeLong * DEGS2RADS, longPeri);
	double AscNodeR = OrbAltTA(Object.Eccentricity, SMA, AscNodeTA);
	Vector AscNode = coordsFromTA(AscNodeR, AscNodeTA);

	Vector rotAN = Rotate2D(&AscNode, rotAngle);

	KeplerCoords2D_t AscNode2D = *(KeplerCoords2D_t*)rotAN.data;

	Vector unitVector = unitVector2D(AscNode2D.x, AscNode2D.y);

	deallocVector(&AscNode);
	deallocVector(&rotAN);

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
	} else {
		epoch_year += 1900;
	}

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

	Vector current2D = coordsFromTA(Current_R, Current_TA * DEGS2RADS);
	Vector rotCurrent2D = Rotate2D(&current2D, rotAngle);
	Vector current3D = Rotate3D(&unitVector, &rotCurrent2D, Object.Inclination * DEGS2RADS);

	KeplerCoords3D_t coords3D = *(KeplerCoords3D_t*)current3D.data;

	deallocVector(&current2D);
	deallocVector(&rotCurrent2D);
	deallocVector(&current3D);

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