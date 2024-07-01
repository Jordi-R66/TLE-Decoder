#include "Common.h"

#define EARTH_DAY_LENGTH 86400.0//86164.096

#define G 6.67428E-11
#define EARTH_MASS 5.9722E24
#define POLAR_RADIUS 6356752
#define EQUA_RADIUS 6378137

#define EARTH_RADIUS ((2*EQUA_RADIUS + POLAR_RADIUS)/3) /*6371000*/
// #define EARTH_RADIUS 6371000

#define EARTH_MU (G*EARTH_MASS)

#undef M_PI
#define M_PI 3.141592653589793

#define DEGS2RADS M_PI/180.0
#define RADS2DEGS 180.0/M_PI

double OrbitalPeriod(double MeanMotion);

uint64_t SemiMajorAxis(double Period);

uint64_t Apoapsis(double Eccentricity, uint64_t SemiMajorAxis);
uint64_t Periapsis(double Eccentricity, uint64_t SemiMajorAxis);
uint64_t OrbAlt(double Eccentricity, uint64_t SemiMajorAxis, double EccentricAnomaly);
uint64_t OrbAltTA(double Eccentricity, uint64_t SemiMajorAxis, double TrueAnomaly);

double KeplerEquation(double E, double e);
double KeplerPrime(double E, double e);

double AngularSpeed(double SemiMajorAxis);
double TrueAnomaly(double Eccentricity, double EccentricAnomaly);

double OrbSpeed(uint64_t altitude, uint64_t SemiMajorAxis);
double Lorentz(double speed);