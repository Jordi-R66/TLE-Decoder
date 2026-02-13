#pragma once
#include "Common.h"

#ifndef CONVERSIONS
#define CONVERSIONS

uint32_t ipow(uint32_t, uint32_t);

uint32_t strint(char*);

double strtoscinotd(char*);

string secstohms(double secs);

#undef M_PI
#define M_PI 3.141592653589793

#define DEGS2RADS M_PI/180.0
#define RADS2DEGS 180.0/M_PI

#endif