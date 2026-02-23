#include "Common.h"
#include "OrbMaths.h"
#include "Algos.h"
#include "Kepler.h"

#pragma region Static Phase

/**
 * @brief Converts mean motion from revolutions per day to radians per second
 *
 * @param meanMotion in revolutions per day
 * @return meanMotion in radians per second
 */
double convertMeanMotion(double meanMotion) {
	static const double CONVERSION_FACTOR = (2.0 * M_PI) / MEAN_SOLAR_DAY;
	return meanMotion * CONVERSION_FACTOR;
}

/**
 * @brief Finds the orbital period by using the Mean Motion (rev / day)
 *
 * @param meanMotion in revolutions per day
 * @return orbital period in seconds
 */
double orbPeriod(double meanMotion) {
	return MEAN_SOLAR_DAY / meanMotion;
}

/**
 * @brief Finds the semi major axis using the mean motion in radians per second
 *
 * @param meanMotion in radians per second
 * @return semi major axis in meters
 */
double semiMajorAxis(double meanMotion) {
	//double temp = MEAN_SOLAR_DAY / (2.0 * M_PI);
	return cbrt(MU_EARTH / (meanMotion * meanMotion));
}

#pragma endregion

#pragma region Dynamic Phase

double EccentricAnomaly(double e, double MA) {
	double E_Approx = MA + e * sin(MA);
	double E = NewtonRaphson(
		MA, e, *KeplerEquation, *KeplerPrime, E_Approx, 1E-6, 1000
	);

	return E;
}

double TrueAnomaly(double E, double e) {
	double beta = e / (1.0 + sqrt(1.0 - e * e));
	double nu = E + 2.0 * atan(
		beta * sin(E) /
		(1 - beta * cos(E))
	);

	return nu;
}

double AltFromTA(double a, double e, double nu) {
	return a * (1 - e * e) / (1.0 + e * cos(nu));
}

#pragma endregion