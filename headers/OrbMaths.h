#pragma once

#include "Common.h"
#include "Constants.h"

#pragma region Static Phase

/**
 * @brief Converts mean motion from revolutions per day to radians per second
 * 
 * @param meanMotion in revolutions per day
 * @return `double` mean motion in radians per second
 */
double convertMeanMotion(double meanMotion);

/**
 * @brief Finds the orbital period by using the Mean Motion (rev / day)
 * 
 * @param meanMotion in revolutions per day
 * @return orbital period in seconds 
 */
double orbPeriod(double meanMotion);

/**
 * @brief Finds the semi major axis using the mean motion in radians per second
 * 
 * @param meanMotion in radians per second
 * @return semi major axis in meters
 */
double semiMajorAxis(double meanMotion);

#pragma endregion

#pragma region Dynamic Phase

double EccentricAnomaly(double MA, double e);
double TrueAnomaly(double E, double e);

double AltFromTA(double a, double e, double nu);

#pragma endregion