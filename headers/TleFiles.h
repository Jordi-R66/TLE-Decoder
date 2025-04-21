#pragma once
#include "TleParser.h"
#include "Common.h"

int32_t GetTLENumber(string* filename);

TLE* GetAllTle(string* filename);
TLE GetSingleTLE(string* filename, uint32_t NORAD_ID);

void PrintContentAsAscii(string* filename);

void exportSingleTLE(string* filename, TLE* tlePtr);
void importSingleTLE(string* filename, TLE* tlePtr);