#include "headers/Aggreg.h"


int main(int argc, char** argv) {

	if (argc == 3) {
		char* filename = argv[1];
		uint32_t noradId = strint(argv[2]);

		TLE tle = initPhase(filename, noradId);
		StaticValues staticVals = staticPhase(tle);

		AggregValues aggreg = {.init = staticVals, .tle=tle};

		while (true) {
			time_t current_ts = time(NULL);
			DynamicValues dynVals = dynamicPhase(tle, staticVals, current_ts);

			aggreg.values_at_time = dynVals;
			aggreg.timestamp = current_ts;

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