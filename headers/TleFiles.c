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

	fread(&output, TLE_BLOCK_SIZE, 1, fp);
	fseek(fp, curr_pos, SEEK_SET);

	output.FIRST_LINE[24] = '\0';
	output.SECOND_LINE[69] = '\0';
	output.THIRD_LINE[69] = '\0';

	return output;
};

TLE parse_block(tle_block* block) {
	return parse_lines(block->FIRST_LINE, block->SECOND_LINE, block->THIRD_LINE);
}

uint32_t readNoradIdFromBlock(tle_block* block) {
	uint32_t noradId = 0;

	char NORAD_CAT[NORAD_ID_LENGTH] = {0};

	for (uint8_t i = 0; i < NORAD_ID_LENGTH; i++) {
		if (i < 5) {
			NORAD_CAT[i] = block->SECOND_LINE[i + 2];
		}
	}

	noradId = (uint32_t)strint(NORAD_CAT);

	return noradId;
}

TLE GetSingleTLE(FILE* fp, uint32_t noradId) {
	TLE output = {0};

	tle_block tempBlock;
	long tle_count = GetTLENumber(fp);
	printf("Counted the TLEs (%ld)\n", tle_count);

	bool found = false;
	long i = 0;

	while (!found && i < tle_count) {
		tempBlock = getBlockByIndex(fp, i);
		uint32_t temp_noradId = readNoradIdFromBlock(&tempBlock);

		found = noradId == temp_noradId;

		if (found) {
			fprintf(stdout, "%s\n%s\n%s\n\n", tempBlock.FIRST_LINE, tempBlock.SECOND_LINE, tempBlock.THIRD_LINE);
		}

		i++;
	}

	output = parse_block(&tempBlock);

	return output;
}

/*
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
*/