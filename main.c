#include "headers/TleFiles.h"
#include "headers/TermFuncs.h"
#include "headers/TimeFuncs.h"

#include "headers/Intermediate.h"

#define CHECKSUM_MODULO 10

cstring filename = NULL;
uint32_t lookingFor = 0;
bool ArgsGiven = false;

void ArgumentParser(char** argv, int argc) {
	char* endptr;

	if (argc == 3) {
		ArgsGiven = true;

		filename = argv[1];
		lookingFor = strtoul(argv[2], &endptr, 10);
	} else {
		fprintf(stderr, "Please pass file as first argument and NORAD ID as second argument\n");
		exit(EXIT_FAILURE);
	}
}

int32_t main(int argc, char* argv[]) {
	ArgumentParser(argv, argc);
	clear();

	const bool ReadingFile = false;

	if (!ReadingFile) {

		/*TLE* AllObjs = GetAllTle(filename);
		int32_t block_quant = GetTLENumber(filename);

		printf("%d entries loaded\nLooping to find %u\n\n", block_quant, lookingFor);

		TLE CurrentEntry;
		bool found = false;

		for (int32_t i = 0; i < block_quant; i++) {
			CurrentEntry = AllObjs[i];
			if (CurrentEntry.NORAD_ID == lookingFor) {
				found = true;
				break;
			}
		}

		free(AllObjs);

		if (!found) {
			printf("Unable to find %u amongst the TLEs\n", lookingFor);
			exit(-1);
		}*/

		TLE CurrentEntry = GetSingleTLE(filename, lookingFor);

		//exportSingleTLE("25544.bin", &CurrentEntry);

		while (true) {
			clear();
			PrintTle(&CurrentEntry);
			sleep(1);
		}
	} else {
		printf("Opening the files\n");
		int32_t block_number = GetTLENumber(filename);

		time_t rawtime_start;
		time_t rawtime_end;

		time(&rawtime_start);

		TLE* AllObjs = GetAllTle(filename);

		time(&rawtime_end);

		free(AllObjs);

		printf("Memory usage to store %d blocks : %lu bytes\nTime : %ld\n", block_number, sizeof(TLE) * block_number, rawtime_end - rawtime_start);
	}

	return 0;
}