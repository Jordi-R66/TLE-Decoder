#include "../headers/Algos.h"

int32_t checksum_algorithm(string line, int32_t modulus) {

	int32_t checksum = 0;
	int32_t line_length = strlen(line) + 1;

	if (line_length != 69) {
		return -1;
	}

	for (int32_t i = 0; i < line_length; i++) {
		int8_t c = line[i];

		if ((48 <= c) && (c <= 57)) {
			c -= 48;
		}
		else if (c == '-') {
			c = 1;
		}
		else {
			c = 0;
		}

		checksum += (int32_t)c;
	}

	checksum %= modulus;

	return checksum;
}

double AntecedentDroite(double a, double b, double y) {
	return (y - b) / a;
}

double NewtonRaphson(double target, double func_param, double (*func)(double, double), double (*func_prime)(double, double), double x_start, double tolerance, uint64_t max_iter) {
	double xf, x_guess = x_start;

	double low_limit = target - tolerance;
	double high_limit = target + tolerance;

	for (uint64_t n = 0; (n < max_iter) & !((low_limit <= func(x_guess, func_param)) & (func(x_guess, func_param) <= high_limit)); n++) {
		double a = func_prime(x_guess, func_param);
		double b = -func_prime(x_guess, func_param) * x_guess + func(x_guess, func_param);
		x_guess = AntecedentDroite(a, b, target);
	}

	xf = x_guess;
	return xf;
}
