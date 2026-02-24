#include "StaticPhase.h"
#include "Coords3D.h"

StaticValues staticPhase(TLE tle) {
	StaticValues output = {0};

	output.n = convertMeanMotion(tle.MeanMotion);
	output.a = semiMajorAxis(output.n);
	output.T = orbPeriod(tle.MeanMotion);
	output.M = tle.MeanAnomaly * DEGS2RADS;
	output.p = output.a * (1.0 - tle.Eccentricity * tle.Eccentricity);

	double E = EccentricAnomaly(output.M, tle.Eccentricity);
	double epoch_nu = TrueAnomaly(E, tle.Eccentricity);

	double apo_r, peri_r, epoch_r;

	apo_r = AltFromTA(output.a, tle.Eccentricity, M_PI);
	peri_r = AltFromTA(output.a, tle.Eccentricity, 0.0);
	epoch_r = AltFromTA(output.a, tle.Eccentricity, epoch_nu);

	output.apo_alt = getWGS84AltitudeFromTA(M_PI, output.a, &tle);
	output.peri_alt = getWGS84AltitudeFromTA(0.0, output.a, &tle);
	output.epoch_alt = getWGS84AltitudeFromTA(epoch_nu, output.a, &tle);

	output.apo_spd = orbSpeed(output.a, apo_r);
	output.peri_spd = orbSpeed(output.a, peri_r);
	output.epoch_spd = orbSpeed(output.a, epoch_r);

	output.epoch_timestamp = getEpochTimestampFromTLE(tle);

	return output;
}
