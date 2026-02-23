#include "DynamicPhase.h"

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp) {
	DynamicValues output = { 0 };

	time_t deltaTime = currentTimestamp - init.epoch_timestamp;

	double deltaM = (double)(init.n) * deltaTime;
	double M = fmod(deltaM + init.M, 2.0 * M_PI);

	double E = EccentricAnomaly(M, tle.Eccentricity);
	double nu = TrueAnomaly(E, tle.Eccentricity);
	double r = AltFromTA(init.a, tle.Eccentricity, nu);

	output.ecc_ano = E;
	output.mean_ano = M;
	output.nu = nu;
	output.distanceToFocal = r;

	output.deltaTime = currentTimestamp;

	return output;
}