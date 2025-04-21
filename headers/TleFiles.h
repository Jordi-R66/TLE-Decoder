#pragma once
#include "TleParser.h"
#include "Common.h"

int32_t GetTLENumber(string* filename);

TLE* GetAllTle(string* filename);

void PrintContentAsAscii(string* filename);

void exportSingleTLE(string* filename, TLE* tlePtr);
void importSingleTLE(string* filename, TLE* tlePtr);