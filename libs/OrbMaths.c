#include "OrbMaths.h"

// ------------------------------------------------------------------

// double cbrt(double x) {
// 	return pow(x, 1.0/3.0);
// }

float OrbitalPeriod(double MeanMotion) {
	float Period;

	Period = (float)(EARTH_DAY_LENGTH / MeanMotion);

	return Period;
}

uint64_t SemiMajorAxis(float Period) {
	double SMA;

	SMA = cbrt(pow((Period / (2.0 * M_PI)), 2.0) * EARTH_MU);

	//SMA = cbrt((G * EARTH_MASS * pow(Period, 2))/(4.0*pow(M_PI, 2.0)));

	if ((SMA >= 0) & (SMA <= 0xffffffffffffffffULL)) {
		return (uint64_t)SMA;
	} else {
		return 0;
	}
}



uint64_t Apogee(float Eccentricity, uint64_t SemiMajorAxis) {
	uint64_t Apogee = 0;

	Apogee = (uint64_t)(SemiMajorAxis * (1.0f + Eccentricity));

	return Apogee;
}

uint64_t Perigee(float Eccentricity, uint64_t SemiMajorAxis) {
	uint64_t Perigee = 0;

	Perigee = (uint64_t)(SemiMajorAxis * (1.0f - Eccentricity));

	return Perigee;
}

uint64_t OrbAlt(float Eccentricity, uint64_t SemiMajorAxis, double E) {
	return (uint64_t)(SemiMajorAxis * (1.0 - Eccentricity * cos(E)));
}



double KeplerEquation(double E, double e) {
	return E - e * sin(E);
}

double KeplerPrime(double E, double e) {
	return 1.0 - e * cos(E);
}

double AngularSpeed(double SemiMajorAxis) {
	return sqrt(EARTH_MU/pow(SemiMajorAxis, 3));
}

double TrueAnomaly(float Eccentricity, double EccentricAnomaly) {
	return 2.0 * atan(sqrt((1.0f+Eccentricity)/(1.0f-Eccentricity) * tan(EccentricAnomaly/2.0)));
}

double OrbSpeed(uint64_t altitude, uint64_t SemiMajorAxis) {
	double speed;

	speed = sqrt(EARTH_MU * (2/((double)altitude) - 1/((double)SemiMajorAxis)));

	return speed;
}

double Lorentz(double v) {
	const double c_sqr = pow(299792458.0, 2.0);

	return 1.0 / sqrt(1 - pow(v, 2.0)/c_sqr);
}