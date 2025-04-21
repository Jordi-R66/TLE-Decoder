#pragma once

#include "TleParser.h"
#include "OrbMaths.h"
#include "Algos.h"
#include "Kepler.h"

#include "../libs/myOwnCLib/vectors/vectors.h"

#define DEFAULT_ITER 100000
#define CALENDAR_YEAR 365.25

void PrintTle(TLE Object);