#include "TleFiles.h"

#include <sys/stat.h>

int32_t GetTLENumber(string* filename) {
	FILE* file = fopen(filename, "r");
	char c = 'a';

	int32_t line_count = 0;

	while (c != EOF) {
		c = getc(file);
		if (c == '\n') {
			line_count++;
		}
	}

	fclose(file);

	return line_count / 3;
}

TLE* GetAllTle(string* filename) {

	uint32_t TLE_NUMBER = GetTLENumber(filename);
	const uint8_t block_size = 165;

	FILE* tle_file = fopen(filename, "r");

	TLE* Output = (TLE*)malloc(sizeof(TLE) * TLE_NUMBER);

	if (Output == NULL) {
		return NULL;
	}

	memset(Output, 0, sizeof(TLE) * TLE_NUMBER);

	string first_line[25];
	string second_line[70];
	string third_line[70];

	for (uint32_t block_number = 0; block_number < TLE_NUMBER; block_number++) {

		uint8_t block_line = 0;
		uint8_t j = 0;

		for (uint8_t i = 0; i < block_size; i++) {
			uint32_t pos = block_number * block_size + i;

			if (i >= 25) {
				block_line = 1;
				j = i - 25;
			}

			if (i >= 95) {
				block_line = 2;
				j = i - 95;
			}

			fseek(tle_file, pos, SEEK_SET);
			char c = fgetc(tle_file);

			if ((c == '\n') || (c == EOF)) {
				c = '\0';
			}

			switch (block_line) {
			case 0:
				first_line[j] = c;
				break;

			case 1:
				second_line[j] = c;
				break;

			case 2:
				third_line[j] = c;
				break;

			default:
				break;
			}
			j++;
		}

		TLE obj = parse_lines(first_line, second_line, third_line);
		Output[block_number] = obj;
	}

	fclose(tle_file);

	return Output;
}

void PrintContentAsAscii(string* filename) {
	FILE* file = fopen(filename, "r");
	char c = 'a';

	while (c != EOF) {
		c = getc(file);
	}

	fclose(file);

	return;
}

void exportSingleTLE(string* filename, TLE* tlePtr) {
	FILE* fp = fopen(filename, "w");

	fwrite(tlePtr, sizeof(*tlePtr), 1, fp);
	fclose(fp);
}

void importSingleTLE(string* filename, TLE* tlePtr) {
	FILE* fp = fopen(filename, "r");

	fread(tlePtr, sizeof(TLE), 1, fp);
	fclose(fp);
}