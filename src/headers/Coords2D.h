#pragma once

#include "Common.h"

typedef struct coords_2d {
	double x, y;
} Coords2D;

#define COORDS_2D_SIZE sizeof(Coords2D)

Coords2D getPlaneCoords(double nu, double r);
double getDistanceFromCoords2D(Coords2D coords);