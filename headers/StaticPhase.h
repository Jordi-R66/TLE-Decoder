#pragma once

#include "Common.h"
#include "TleFiles.h"
#include "OrbMaths.h"
#include "TimeFuncs.h"

typedef struct staticValues {
	double T;				// Periode orbitale en secondes
	double n;				// Moyen Mouvement en rad/s
	double a;				// Demi grand-axe en mètres
	double M;				// Moyenne anomalie en radians
	double p;				// p = a * (1.0 - e^2)

	time_t epoch_timestamp;	// Timestamp de la TLE
} StaticValues;

StaticValues staticPhase(TLE tle);
