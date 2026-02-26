#pragma once

#include "Geography.h"
#include "StaticPhase.h"
#include "Coords2D.h"
#include "Coords3D.h"

typedef struct dynamicValues {
	Coords2D coords_2d;
	Coords3D coords_3d;
	GeoCoords geo_coords;

	time_t deltaTime;
	double true_ano, mean_ano, ecc_ano;

	double distanceToFocal;
	double speed;
} DynamicValues;

#define DYN_VALS_SIZE sizeof(DynamicValues)

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp);
