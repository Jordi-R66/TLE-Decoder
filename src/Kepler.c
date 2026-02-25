#include "Kepler.h"

double KeplerEquation(double EccentricAnomaly, double Eccentricity) {
	return EccentricAnomaly - Eccentricity * sin(EccentricAnomaly);
}

double KeplerPrime(double EccentricAnomaly, double Eccentricity) {
	return 1.0 - Eccentricity * cos(EccentricAnomaly);
}