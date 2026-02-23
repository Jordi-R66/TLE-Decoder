#pragma once

#include "StaticPhase.h"

typedef struct dynamicValues {
	time_t deltaTime;
	double nu, mean_ano, ecc_ano;
} DynamicValues;

DynamicValues dynamicPhase(TLE tle, StaticValues init, time_t currentTimestamp);
