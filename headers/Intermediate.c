#include "Intermediate.h"
#include "TimeFuncs.h"
#include "Kepler.h"

double EccentricAnomalyTolerance = 1E-5 * DEGS2RADS;

time_t current_time;

bool uvInit = false; // Has the unit vector been initialized?
//Vector uv; // Unit vector

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

	output.epoch = tleToDate(epoch_year, Object->EPOCH);

	return output;
}

EpochData computeEpochData(TLE* Object, OrbitData* orbitData, bool realTime) {
	EpochData output;

	memset(&output, 0, EPOCH_DATA_LENGTH);

	double deltaTime = 0.0;
	output.epoch = orbitData->epoch;

	if (realTime) {
		output.epoch = CurrentDate();
		deltaTime = timeDelta(output.epoch, orbitData->epoch);
	}

	double MA = Object->MeanAnomaly + (orbitData->n * deltaTime * RADS2DEGS);
	MA = fmod(MA, 360.0);

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

	return output;
}

void PrintTle(TLE* Object) {
	OrbitData orbitData = computeOrbitData(Object);

	EpochData epoch = computeEpochData(Object, &orbitData, false);
	EpochData current = computeEpochData(Object, &orbitData, true);

	double DeltaTime = timeDelta(current.epoch, epoch.epoch);

	Date utc = current.epoch;

	double Speed_Ap, Speed_Pe;

	Speed_Ap = OrbSpeed(orbitData.Ap, orbitData.SMA);
	Speed_Pe = OrbSpeed(orbitData.Pe, orbitData.SMA);

	printf("Object name : %s\n", Object->name);

	printf("---------------------------------- TLE ----------------------------------\n");

	printf("NORAD ID : %0*u%c\n", 5, Object->NORAD_ID, Object->Classification);
	printf("COSPAR : %0*d %0*hu %s\n", 2, Object->COSPAR_YR, 3, Object->COSPAR_LN, Object->COSPAR_ID);
	printf("EPOCH : YEAR=20%d DAY=%.8lf\n", Object->EPOCH_YR, Object->EPOCH);
	printf("TLE AGE : %s\n", secstohms(DeltaTime));
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
	printf("Apoapsis : %.0lf m | Periapsis : %.0lf m | Epoch : %.0lf m\n", orbitData.Ap - (double)EARTH_RADIUS, orbitData.Pe - (double)EARTH_RADIUS, epoch.Alt);
	printf("Speed @ Ap : %.4lf m/s | Pe : %.4lf m/s | Ep : %.4lf m/s \n", Speed_Ap, Speed_Pe, epoch.Spd);

	printf("------------------------------- CURRENTLY -------------------------------\n");

	printf("DATE (UTC) : %0*d/%0*d/%0*d %0*d:%0*d:%0*d\n", 2, utc.Day, 2, utc.Month + 1, 4, utc.Year, 2, utc.Hour, 2, utc.Minute, 2, utc.Second);
	printf("ALTITUDE : %.0lf m\n", current.Alt);
	printf("SPEED : %.2lf m/s\n", current.Spd);

	printf("-------------------------------------------------------------------------\n");

	uvInit = false;
}