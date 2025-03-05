#include "Ellipses.h"

double semiMinorAxis(double semiMajorAxis, double eccentricity) {
	return semiMajorAxis * sqrt(1.0 - pow(eccentricity, 2.0));
}

double c(double semiMajorAxis, double eccentricity) {
	return semiMajorAxis * eccentricity;
}

double h(double semiMajorAxis, double eccentricity) {
	return p(semiMajorAxis, eccentricity) / eccentricity;
}

double f(double semiMajorAxis, double eccentricity) {
	return semiMajorAxis / eccentricity;
}

double p(double semiMajorAxis, double eccentricity) {
	return semiMajorAxis * (1.0 - pow(eccentricity, 2.0));
}