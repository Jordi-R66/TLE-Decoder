#pragma once

#include "Common.h"
#include "TleFiles.h"
#include "OrbMaths.h"

typedef struct staticValues {
	double T; // Periode orbitale en secondes
	double n; // Moyen Mouvement en rad/s
	double a; // Demi grand-axe en mètres
} StaticValues;

StaticValues staticPhase(TLE tle);
