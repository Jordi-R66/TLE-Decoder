#pragma once

#include "TleParser.h"
#include "OrbMaths.h"
#include "Algos.h"
#include "Kepler.h"

#include "../libs/myOwnCLib/vectors/vectors.h"

#define DEFAULT_ITER 100000
#define CALENDAR_YEAR 365.25

typedef struct OrbitData {
	double OrbPeriod;
	double SMA;
	double n;

	double Ap, Pe;
	double longPeri;
	double epoch;
} OrbitData;

typedef struct EpochData {
	double E;
	double MA;
	double TA;

	double R;
	double Alt;
	double Spd;

	KeplerCoords2D_t coords2D;
	KeplerCoords3D_t coords3D;
} EpochData;

#define ORBIT_DATA_LENGTH sizeof(OrbitData)
#define EPOCH_DATA_LENGTH sizeof(EpochData)

OrbitData computeOrbitData(TLE Object);

void PrintTle(TLE Object);