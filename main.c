#include "headers/StaticPhase.h"

TLE initPhase(char* filename, uint32_t noradId) {
	FILE* fp = fopen(filename, "r");
	TLE tle = GetSingleTLE(fp, noradId);
	fclose(fp);

	return tle;
}

int main(int argc, char** argv) {
	char* filename = argv[1];
	uint32_t noradId = strint(argv[2]);

	TLE tle = initPhase(filename, noradId);
	StaticValues staticVals = staticPhase(tle);

	double* vals = (double*)&staticVals;

	for (int i = 0; i < 3; i++) {
		printf("%lf\n", vals[i]);
	}

	return 0;
}