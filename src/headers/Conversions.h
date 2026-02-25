#pragma once
#include "Common.h"

#ifndef CONVERSIONS
#define CONVERSIONS

uint32_t ipow(uint32_t, uint32_t);

uint32_t strint(char*);

double strtoscinotd(char*);

string secstohms(double secs);

extern const double DEGS2RADS;
extern const double RADS2DEGS;

#endif