#pragma once
#include "Common.h"

int32_t GetTLENumber(string* filename);

TLE* GetAllTle(string* filename);

void PrintContentAsAscii(string* filename);

void exportSingleTLE(string* filename, TLE tleObject);
void importSingleTLE(string* filename, TLE* tlePtr);