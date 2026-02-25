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

long GetTLENumber(FILE* fp);

TLE parse_block(tle_block* block);

TLE GetSingleTLE(FILE* fp, uint32_t NORAD_ID);

/*
void PrintContentAsAscii(string filename);

void exportSingleTLE(string filename, TLE* tlePtr);
void importSingleTLE(string filename, TLE* tlePtr);
*/