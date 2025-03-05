#include "Ellipses.h"

double b(double a, double e) {
	return a * sqrt(1.0 - pow(e, 2.0));
}

double c(double a, double e) {
	return a * e;
}

double h(double a, double e) {
	return p(a, e) / e;
}

double f(double a, double e) {
	return a / e;
}

double p(double a, double e) {
	return a * (1.0 - pow(e, 2.0));
}