#pragma once

#include "Common.h"
#include "Coords2D.h"
#include "TleParser.h"

// ECI - Earth Centered Referential
typedef struct coords_3d {
	double x, y, z;
} Coords3D;

Coords3D getECICoords(Coords2D planeCoords, TLE* tle);
double getDistanceFromCoords3D(Coords3D coords);