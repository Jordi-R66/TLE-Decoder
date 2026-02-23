#include "StaticPhase.h"

StaticValues staticPhase(TLE tle) {
	StaticValues output = {0};

	output.n = convertMeanMotion(tle.MeanMotion);
	output.a = semiMajorAxis(output.n);
	output.T = orbPeriod(tle.MeanMotion);
	output.M = tle.MeanAnomaly * DEGS2RADS;

	output.epoch_timestamp = getEpochTimestampFromTLE(tle);

	return output;
}
