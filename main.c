#include "headers/Loops.h"


int main(int argc, char** argv) {

	if (argc == 3) {
		char* filename = argv[1];
		uint32_t noradId = strint(argv[2]);

		realtimeLoop(filename, noradId, printValues, stdout);
	} else {
		fprintf(stderr, "Please provide 2 arguments\n\t%s TLE_FILE NORAD_ID\n", argv[0]);
		return -1;
	}

	return 0;
}