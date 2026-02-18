#include "TleFiles.h"
#include "Conversions.h"

#include <sys/stat.h>

const uint8_t block_size = 165;

long GetTLENumber(FILE* fp) {
	long curr_pos = ftell(fp);

	fseek(fp, 0, SEEK_END);
	long filesize = ftell(fp);
	fseek(fp, curr_pos, SEEK_SET);

	return filesize / TLE_BLOCK_SIZE;
}

tle_block getBlockByIndex(FILE* fp, long index) {
	tle_block output = {0};

	long curr_pos = ftell(fp);

	long new_pos = TLE_BLOCK_SIZE * index;
	fseek(fp, new_pos, SEEK_SET);

	size_t read_bytes = fread(&output, TLE_BLOCK_SIZE, 1, fp);
	fseek(fp, curr_pos, SEEK_SET);

	return output;
}

TLE GetSingleTLE(string filename, uint32_t NORAD_ID) {
	uint32_t TLE_NUMBER = GetTLENumber(filename);
	FILE* tle_file = fopen(filename, "r");

	TLE Output;
	memset(&Output, 0, sizeof(Output));

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

		Output = parse_lines(first_line, second_line, third_line);

		if (Output.NORAD_ID == NORAD_ID) {
			fclose(tle_file);
			return Output;
		}
	}

	fclose(tle_file);
	fprintf(stderr, "Error: Couldn't find %u in %s\n", NORAD_ID, filename);
	exit(EXIT_FAILURE);
}

void PrintContentAsAscii(string filename) {
	FILE* file = fopen(filename, "r");
	char c = 'a';

	while (c != EOF) {
		c = getc(file);
	}

	fclose(file);

	return;
}

void exportSingleTLE(string filename, TLE* tlePtr) {
	FILE* fp = fopen(filename, "w");

	fwrite(tlePtr, sizeof(*tlePtr), 1, fp);
	fclose(fp);
}

void importSingleTLE(string filename, TLE* tlePtr) {
	FILE* fp = fopen(filename, "r");

	fread(tlePtr, sizeof(TLE), 1, fp);
	fclose(fp);
}