#include "OrbMaths.h"
#include "Ellipses.h"

// ------------------------------------------------------------------

// double cbrt(double x) {
// 	return pow(x, 1.0/3.0);
// }

double OrbitalPeriod(double MeanMotion) {
	double Period;

	Period = EARTH_DAY_LENGTH / MeanMotion;

	return Period;
}

double SemiMajorAxis(double Period) {
	double SMA;

	SMA = cbrt(pow((Period / (2.0 * M_PI)), 2.0) * EARTH_MU);

	//SMA = cbrt((G * EARTH_MASS * pow(Period, 2))/(4.0*pow(M_PI, 2.0)));

	return SMA;
}



double Apoapsis(double Eccentricity, double SemiMajorAxis) {
	double Apoapsis = 0;

	Apoapsis = SemiMajorAxis * (1.0 + Eccentricity);

	return Apoapsis;
}

double Periapsis(double Eccentricity, double SemiMajorAxis) {
	double Periapsis = 0;

	Periapsis = SemiMajorAxis * (1.0 - Eccentricity);

	return Periapsis;
}

double OrbAlt(double Eccentricity, double SemiMajorAxis, double EccentricAnomaly) {
	return SemiMajorAxis * (1.0 - Eccentricity * cos(EccentricAnomaly));
}

double OrbAltTA(double Eccentricity, double SemiMajorAxis, double TrueAnomaly) {
	double a = p(SemiMajorAxis, Eccentricity);
	double b = 1.0 + Eccentricity * cos(TrueAnomaly);

	return a / b;
}



double KeplerEquation(double EccentricAnomaly, double Eccentricity) {
	return EccentricAnomaly - Eccentricity * sin(EccentricAnomaly);
}

double KeplerPrime(double EccentricAnomaly, double Eccentricity) {
	return 1.0 - Eccentricity * cos(EccentricAnomaly);
}

double AngularSpeed(double SemiMajorAxis) {
	return sqrt(EARTH_MU / pow(SemiMajorAxis, 3));
}

double TrueAnomaly(double Eccentricity, double EccentricAnomaly) {
	// return 2.0 * atan(sqrt((1.0 + Eccentricity) / (1.0 - Eccentricity)) * tan(EccentricAnomaly/2.0));
	double beta_ = Eccentricity / (1.0 + sqrt(1.0 - pow(Eccentricity, 2.0)));
	double nu_ = EccentricAnomaly + 2.0 * atan((beta_ * sin(EccentricAnomaly)) / (1.0 - beta_ * cos(EccentricAnomaly)));

	return nu_;
}

double EccentricAnomaly(double Eccentricity, double TrueAnomaly) {
	double tan_ = tan(TrueAnomaly / 2.0);
	double sqrt_ = sqrt((1.0 + Eccentricity) / (1.0 - Eccentricity));
	double EccentricAnomaly = 2.0 * atan(tan_ / sqrt_);

	return EccentricAnomaly;
}



double OrbSpeed(double altitude, double SemiMajorAxis) {
	double speed;

	speed = sqrt(EARTH_MU * (2.0 / (altitude - 1.0 / SemiMajorAxis)));

	return speed;
}

double Lorentz(double v) {
	const double c_sqr = pow(299792458.0, 2.0);

	return 1.0 / sqrt(1 - pow(v, 2.0) / c_sqr);
}
