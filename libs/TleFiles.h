#pragma once
#include "Common.h"

int32_t GetTLENumber(string *filename);

TLE* GetAllTle(string *filename);

void PrintContentAsAscii(string *filename);

void ExportToStructFile(TLE* tle_list, int32_t tle_number, string* filename);

TLE* ImportFromStructFile(string *filename, int32_t *tle_number);