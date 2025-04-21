#pragma once
#include "Common.h"

#define EARTH_DAY_LENGTH 86400.0//86164.096

#define G 6.67428E-11
#define EARTH_MASS 5.9722E24
#define POLAR_RADIUS 6356752.0
#define EQUA_RADIUS 6378137.0

#define EARTH_RADIUS ((2.0*EQUA_RADIUS + POLAR_RADIUS)/3.0) /*6371000*/
// #define EARTH_RADIUS 6371000

#define EARTH_MU (G*EARTH_MASS)

double OrbitalPeriod(double MeanMotion);

double SemiMajorAxis(double Period);

double Apoapsis(double Eccentricity, double SemiMajorAxis);
double Periapsis(double Eccentricity, double SemiMajorAxis);
double OrbAlt(double Eccentricity, double SemiMajorAxis, double EccentricAnomaly);
double OrbAltTA(double Eccentricity, double SemiMajorAxis, double TrueAnomaly);

double KeplerEquation(double E, double e);
double KeplerPrime(double E, double e);

double AngularSpeed(double SemiMajorAxis);
double TrueAnomaly(double Eccentricity, double EccentricAnomaly);
double EccentricAnomaly(double Eccentricity, double TrueAnomaly);

double OrbSpeed(double altitude, double SemiMajorAxis);
double Lorentz(double speed);