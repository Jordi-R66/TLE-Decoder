#pragma once

#include "StaticPhase.h"

typedef struct dynamicValues {
	time_t timestamp;
	double nu, mean_ano;
} DynamicValues;

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp);
