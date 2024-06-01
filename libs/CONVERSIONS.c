#include "CONVERSIONS.h"
#include <math.h>

uint32_t ipow(uint32_t a, uint32_t b) {
	uint32_t result = 1;
	for (uint32_t i=0; i < b; i++) {
		result *= a;
	}

	return result;
}

uint32_t strint(string *value) {
	uint32_t output_value = 0;
	uint32_t length = strlen(value);

	for (uint32_t i=0; i < length; i++) {
		char c = value[i];
		if (!((c >= 48) && (c <= 57))) {
			return -1;
		}
	}

	for (uint32_t i=0; i < length; i++) {
		char c = value[length-i-1];
		output_value += ((uint32_t)(c - 48)) * ipow(10, i);
	}

	return output_value;
}

double strtoscinotd(string *original) {
	// Converts a string to a scientific notation and then converts it to a double

	int8_t total_shift = 0;

	string intermediate_string[10];
	char *endptr;

	for (uint8_t i=0; i<8; i++) {
		char c = original[i];

		if (((c == '+') || (c == '-')) && i > 0) {
			total_shift = 1;
			intermediate_string[i] = 'e';
		}

		intermediate_string[i+total_shift] = c;
	}

	intermediate_string[9] = '\0';

	double final_output = strtod(intermediate_string, &endptr) * 0.1;

	return final_output;
}

string* secstohms(double secs) {
	static string output[28];

	uint8_t days, hours, minutes, seconds, centiseconds;
	double d_days, d_hours, d_minutes, d_seconds;

	d_days = secs/86400.0;
	days = (uint8_t)d_days;

	d_hours = (d_days - days) * 24.0;
	hours = (uint8_t)d_hours;

	d_minutes = (d_hours - hours) * 60.0;
	minutes = (uint8_t)d_minutes;
	
	d_seconds = (d_minutes - minutes) * 60.0;
	seconds = (uint8_t)d_seconds;

	centiseconds = (uint8_t)((d_seconds - seconds) * 100);

	sprintf(output, "%ud %uh %um %u.%us", days, hours, minutes, seconds, centiseconds);
	return output;
}

string* intstr(uint32_t x) {
	static string output[11];

	if (x == 0) {
		return "0\0";
	}

	uint8_t highest_exponent = (uint8_t)log10((double)x);

	for (uint8_t n=highest_exponent; n>=0; n++) {
		uint8_t i = highest_exponent-n;

		output[i] = (char)((double)x/pow(10.0, (double)n));
	}

	return output;
}