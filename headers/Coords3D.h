#pragma once

#include "Common.h"
#include "Constants.h"
#include "Coords2D.h"
#include "TleParser.h"

// ECI - Earth Centered Referential
typedef struct coords_3d {
	double x, y, z;
} Coords3D;

Coords3D getECICoords(Coords2D planeCoords, TLE* tle);
Coords3D getECICoordsFromTA(double nu, double a, double e, TLE* tle);
double getDistanceFromCoords3D(Coords3D coords);
double getWGS84Altitude(Coords3D coords);
double getWGS84AltitudeFromTA(double nu, double a, double e, TLE* tle);