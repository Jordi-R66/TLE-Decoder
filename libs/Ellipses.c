#include "Ellipses.h"

#include <math.h>

float b(float a, float e) {
	if (!((0 <= e) && (e < 1))) {
		perror("Eccentricity must be 0 <= e < 1");
	}

	return a * sqrt(1 - e*e);
}

float c(float a, float e) {
	if (!((0 <= e) && (e < 1))) {
		perror("Eccentricity must be 0 <= e < 1");
	}

	return a * e;
}

float h(float a, float e) {
	if (!((0 <= e) && (e < 1))) {
		perror("Eccentricity must be 0 <= e < 1");
	}

	return (a * (1.0f - e*e)) / e;
}

float f(float a, float e) {
	if (!((0 <= e) && (e < 1))) {
		perror("Eccentricity must be 0 <= e < 1");
	}

	return a / e;
}

float p(float a, float e) {
	if (!((0 <= e) && (e < 1))) {
		perror("Eccentricity must be 0 <= e < 1");
	}

	return a * (1 - e*e);
}

// ---------------------------------------------------------------------

float r(float TrueAnomaly, float p, float e) {
	if (!((0 <= e) && (e < 1))) {
		perror("Eccentricity must be 0 <= e < 1");
	}

	return p / (1 + e * cos(TrueAnomaly));
}

float InitialX_2D(float TrueAnomaly, float Dist) {
	if (Dist <= 0) {
		perror("Distance must be strictly higher than 0");
	}

	return Dist * cos(TrueAnomaly);
}

float InitialY_2D(float TrueAnomaly, float Dist) {
	if (Dist <= 0) {
		perror("Distance must be strictly higher than 0");
	}

	return Dist * sin(TrueAnomaly);
}

float ActualX_2D(float x, float y, float PeAng) {
	return x * cos(PeAng) - y * sin(PeAng);
}

float ActualY_2D(float x, float y, float PeAng) {
	return x * sin(PeAng) + y * cos(PeAng);
}