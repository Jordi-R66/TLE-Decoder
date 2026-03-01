#include "headers/Loops.h"

int main(int argc, char** argv) {
	int return_code = 0;

	if (argc == 3) {
		char* filename = argv[1];
		uint32_t noradId = strint(argv[2]);

		realtimeLoop(filename, noradId, printValues, stdout);
	} else {
		fprintf(stderr, "Please provide 2 arguments\n\t%s TLE_FILE NORAD_ID\n", argv[0]);
		return_code = -1;
	}

	return return_code;
}