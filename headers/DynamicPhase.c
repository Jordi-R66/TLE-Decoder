#include "DynamicPhase.h"

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp) {
	DynamicValues output = { 0 };

	time_t deltaTime = currentTimestamp - init.epoch_timestamp;

	double deltaM = (double)(init.n) * deltaTime;
	double M = fmod(deltaM + init.M, 2.0 * M_PI);

	double E = EccentricAnomaly(M, tle.Eccentricity);
	double true_ano = TrueAnomaly(E, tle.Eccentricity);
	double r = AltFromTA(init.a, tle.Eccentricity, true_ano);
	double spd = orbSpeed(init.a, tle.Eccentricity);

	output.ecc_ano = E;
	output.mean_ano = M;
	output.true_ano = true_ano;
	output.distanceToFocal = r;
	output.speed = spd;

	output.deltaTime = currentTimestamp;

	return output;
}