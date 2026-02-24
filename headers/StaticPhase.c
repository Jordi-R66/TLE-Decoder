#include "StaticPhase.h"

StaticValues staticPhase(TLE tle) {
	StaticValues output = {0};

	output.n = convertMeanMotion(tle.MeanMotion);
	output.a = semiMajorAxis(output.n);
	output.T = orbPeriod(tle.MeanMotion);
	output.M = tle.MeanAnomaly * DEGS2RADS;
	output.p = output.a * (1.0 - tle.Eccentricity * tle.Eccentricity);

	double E = EccentricAnomaly(output.M, tle.Eccentricity);
	double epoch_nu = TrueAnomaly(E, tle.Eccentricity);

	output.apo_alt = AltFromTA(output.a, tle.Eccentricity, M_PI);
	output.peri_alt = AltFromTA(output.a, tle.Eccentricity, 0.0);
	output.epoch_alt = AltFromTA(output.a, tle.Eccentricity, epoch_nu);

	output.apo_spd = orbSpeed(output.a, output.apo_alt);
	output.peri_spd = orbSpeed(output.a, output.peri_alt);
	output.epoch_spd = orbSpeed(output.a, output.epoch_alt);

	output.epoch_timestamp = getEpochTimestampFromTLE(tle);

	return output;
}
