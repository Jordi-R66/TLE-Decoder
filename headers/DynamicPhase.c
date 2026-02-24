#include "DynamicPhase.h"

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp) {
	DynamicValues output = { 0 };

	time_t deltaTime = currentTimestamp - init.epoch_timestamp;

	double deltaM = init.n * (double)deltaTime;
	double M = fmod(deltaM + init.M, 2.0 * M_PI);

	double E = EccentricAnomaly(M, tle.Eccentricity);
	double true_ano = TrueAnomaly(E, tle.Eccentricity);
	double r = AltFromTA(init.a, tle.Eccentricity, true_ano);
	double spd = orbSpeed(init.a, r);

	Coords2D coords_2d = getPlaneCoords(true_ano, r);
	Coords3D coords_3d = getECICoords(coords_2d, &tle);

	output.ecc_ano = E;
	output.mean_ano = M;
	output.true_ano = true_ano;
	output.distanceToFocal = r;
	output.speed = spd;

	output.coords_2d = coords_2d;
	output.coords_3d = coords_3d;

	output.deltaTime = deltaTime;

	return output;
}