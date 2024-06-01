#include "Common.h"

typedef enum EOL {
	UNDETERMINED = 0,
	LF = 1,
	CRLF = 2,
	CR = 3,
	LFCR = 4
} EOL;

EOL GetEnding(string *filename);

int32_t GetTLENumber(string *filename);

TLE* GetAllTle(string *filename);

void PrintContentAsAscii(string *filename);

void ExportToStructFile(TLE* tle_list, int32_t tle_number, string* filename);

TLE* ImportFromStructFile(string *filename, int32_t *tle_number);