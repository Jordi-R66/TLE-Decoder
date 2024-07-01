#include "OrbMaths.h"

// ------------------------------------------------------------------

// double cbrt(double x) {
// 	return pow(x, 1.0/3.0);
// }

double OrbitalPeriod(double MeanMotion) {
	double Period;

	Period = (double)(EARTH_DAY_LENGTH / MeanMotion);

	return Period;
}

uint64_t SemiMajorAxis(double Period) {
	double SMA;

	SMA = cbrt(pow((Period / (2.0 * M_PI)), 2.0) * EARTH_MU);

	//SMA = cbrt((G * EARTH_MASS * pow(Period, 2))/(4.0*pow(M_PI, 2.0)));

	if ((SMA >= 0) & (SMA <= 0xffffffffffffffffULL)) {
		return (uint64_t)SMA;
	} else {
		return 0;
	}
}



uint64_t Apoapsis(double Eccentricity, uint64_t SemiMajorAxis) {
	uint64_t Apoapsis = 0;

	Apoapsis = (uint64_t)(SemiMajorAxis * (1.0 + Eccentricity));

	return Apoapsis;
}

uint64_t Periapsis(double Eccentricity, uint64_t SemiMajorAxis) {
	uint64_t Periapsis = 0;

	Periapsis = (uint64_t)(SemiMajorAxis * (1.0 - Eccentricity));

	return Periapsis;
}

uint64_t OrbAlt(double Eccentricity, uint64_t SemiMajorAxis, double EccentricAnomaly) {
	return (uint64_t)(SemiMajorAxis * (1.0 - Eccentricity * cos(EccentricAnomaly)));
}

uint64_t OrbAltTA(double Eccentricity, uint64_t SemiMajorAxis, double TrueAnomaly) {
	return (uint64_t)(SemiMajorAxis * (1.0 - pow(Eccentricity, 2.0)) / (1.0 + Eccentricity * cos(TrueAnomaly)));
}



double KeplerEquation(double EccentricAnomaly, double Eccentricity) {
	return EccentricAnomaly - Eccentricity * sin(EccentricAnomaly);
}

double KeplerPrime(double EccentricAnomaly, double Eccentricity) {
	return 1.0 - Eccentricity * cos(EccentricAnomaly);
}

double AngularSpeed(double SemiMajorAxis) {
	return sqrt(EARTH_MU/pow(SemiMajorAxis, 3));
}

double TrueAnomaly(double Eccentricity, double EccentricAnomaly) {
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
