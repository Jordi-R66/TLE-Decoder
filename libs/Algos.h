#include "Common.h"

double AntecedentDroite(double a, double b, double y);

double NewtonRaphson(double target, double func_param, double (*func)(double, double), double (*func_prime)(double, double), double x_start, double tolerance, uint64_t max_iter);