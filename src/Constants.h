#pragma once

#define SIDEREAL_DAY 86164.096	// Earth sidereal day in seconds
#define MEAN_SOLAR_DAY 86400	// Earth mean solar day in seconds

#define G 6.67428e-11

#define MU_EARTH 3.986004415e14	// Earth µ (TT)
#define EARTH_MASS MU_EARTH / G	// Earth mass

// Rayon équatorial (demi-grand axe de la Terre) en mètres
#define WGS84_A 6378137.0

// Rayon polaire (demi-petit axe de la Terre) en mètres
#define WGS84_B 6356752.314245

#define J2_EARTH 0.0010826359