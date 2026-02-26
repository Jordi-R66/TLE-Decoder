#include "headers/Aggreg.h"


int main(int argc, char** argv) {

	if (argc == 3) {
		char* filename = argv[1];
		uint32_t noradId = strint(argv[2]);

		AggregValues aggreg;

		preloadAggreg(filename, noradId, &aggreg);

		while (true) {
			time_t current_ts = time(NULL);
			changeAggregTimestamp(&aggreg, current_ts);

			clear_screen();
			printValues(stdout, aggreg);

			sleep_hz(60);
		}
	} else {
		fprintf(stderr, "Please provide 2 arguments\n\t%s TLE_FILE NORAD_ID\n", argv[0]);
		return -1;
	}

	return 0;
}