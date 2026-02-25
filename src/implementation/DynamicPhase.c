#include "../headers/DynamicPhase.h"

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp) {
	DynamicValues output = { 0 };

	time_t deltaTime = currentTimestamp - init.epoch_timestamp;

	double i_rad = tle.Inclination * DEGS2RADS;
	double j2_factor = 1.5 * J2_EARTH * (WGS84_A * WGS84_A) / (init.p * init.p) * init.n;

	double dot_omega_node = -j2_factor * cos(i_rad);
	double dot_omega_peri = j2_factor * 0.5 * (5.0 * cos(i_rad) * cos(i_rad) - 1.0);

	tle.AscNodeLong += (dot_omega_node * deltaTime) * RADS2DEGS;
	tle.PeriArg += (dot_omega_peri * deltaTime) * RADS2DEGS;

	tle.AscNodeLong = fmod(tle.AscNodeLong, 360.0);
	if (tle.AscNodeLong < 0.0) tle.AscNodeLong += 360.0;

	tle.PeriArg = fmod(tle.PeriArg, 360.0);
	if (tle.PeriArg < 0.0) tle.PeriArg += 360.0;

	double deltaM = init.n * (double)deltaTime;
	double M = fmod(deltaM + init.M, 2.0 * M_PI);

	double E = EccentricAnomaly(M, tle.Eccentricity);
	double true_ano = TrueAnomaly(E, tle.Eccentricity);
	double r = AltFromTA(init.a, tle.Eccentricity, true_ano);
	double spd = orbSpeed(init.a, r);

	Coords2D coords_2d = getPlaneCoords(true_ano, r);
	Coords3D coords_3d = getECICoords(coords_2d, &tle);
	GeoCoords geo_coords = getGeoCoords(coords_3d, currentTimestamp);

	output.ecc_ano = E;
	output.mean_ano = M;
	output.true_ano = true_ano;
	output.distanceToFocal = r;
	output.speed = spd;

	output.coords_2d = coords_2d;
	output.coords_3d = coords_3d;
	output.geo_coords = geo_coords;

	output.deltaTime = deltaTime;

	return output;
}