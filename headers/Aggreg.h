#pragma once

#include "utils.h"
#include "DynamicPhase.h"

typedef struct {
	TLE tle;
	DynamicValues values_at_time;
	StaticValues init;
	time_t timestamp;
} AggregValues;

#define AGGREG_VALS_SIZE sizeof(AggregValues)

TLE initPhase(char* filename, uint32_t noradId);

AggregValues getAggregAtTimestamp(char* filename, uint32_t norad_id, time_t timestamp);
void changeAggregTimestamp(AggregValues* aggreg, time_t timestamp);

// Type pour les fonctions chargées d'écrire les données dans un flux
// Doit retourner un void, prendre en entrée un pointeur vers une struct FILE et une struct AggregValues
typedef void (*AggregWriter)(FILE*, AggregValues);

void printValues(FILE* stream, AggregValues vals);