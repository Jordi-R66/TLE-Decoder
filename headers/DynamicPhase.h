#pragma once

#include "StaticPhase.h"
#include "Coords2D.h"
#include "Coords3D.h"

typedef struct dynamicValues {
	Coords2D coords_2d;
	Coords3D coords_3d;

	time_t deltaTime;
	double true_ano, mean_ano, ecc_ano;

	double altitude;
	double distanceToFocal;
	double speed;
} DynamicValues;

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp);
