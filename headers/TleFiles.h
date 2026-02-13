#pragma once
#include "TleParser.h"
#include "Common.h"

typedef struct {
	char FIRST_LINE[25];
	char SECOND_LINE[70];
	char THIRD_LINE[70];
} tle_block;

#define TLE_BLOCK_SIZE sizeof(tle_block)

tle_block getBlockByIndex(FILE* fp, long index);

int32_t GetTLENumber(string filename);

TLE GetSingleTLE(string filename, uint32_t NORAD_ID);

void PrintContentAsAscii(string filename);

void exportSingleTLE(string filename, TLE* tlePtr);
void importSingleTLE(string filename, TLE* tlePtr);
