#include "Loops.h"

void realtimeLoop(char* filename, uint32_t noradId, AggregWriter writingFunc, FILE* outputStream) {
	AggregValues aggreg;

	if (writingFunc != (AggregWriter)NULL) {
		preloadAggreg(filename, noradId, &aggreg);

		while (true) {
			time_t current_ts = time(NULL);
			changeAggregTimestamp(&aggreg, current_ts);

			if (writingFunc == printValues) {
				sleep_hz(60);
				clear_screen();
			}

			writingFunc(outputStream, aggreg);
		}
	}
}

void sweepTimestamps(char* filename, uint32_t noradId, AggregWriter writingFunc, FILE* outputStream, time_t start, time_t end, time_t step) {
	AggregValues aggreg;

	if (writingFunc != (AggregWriter)NULL) {
		preloadAggreg(filename, noradId, &aggreg);

		for (time_t timestamp = start; timestamp <= end; timestamp += step) {
			changeAggregTimestamp(&aggreg, timestamp);

			writingFunc(outputStream, aggreg);
		}
	}
}
