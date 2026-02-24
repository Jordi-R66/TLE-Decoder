#include "Coords3D.h"

Coords3D getECICoords(Coords2D planeCoords, TLE* tle) {
	double an = (double)tle->AscNodeLong * DEGS2RADS;
	double arg = (double)tle->PeriArg * DEGS2RADS;
	double inc = (double)tle->Inclination * DEGS2RADS;

	double cos_an = cos(an);
	double sin_an = sin(an);
	double cos_arg = cos(arg);
	double sin_arg = sin(arg);
	double cos_inc = cos(inc);
	double sin_inc = sin(inc);

	double x, y, z;

	x = planeCoords.x * (cos_an * cos_arg - sin_an * sin_arg * cos_inc) + planeCoords.y * (-cos_an * sin_arg - sin_an * cos_arg * cos_inc);
	y = planeCoords.x * (sin_an * cos_arg + cos_an * sin_arg * cos_inc) + planeCoords.y * (-sin_an * sin_arg + cos_an * cos_arg * cos_inc);
	z = planeCoords.x * (sin_arg * sin_inc) + planeCoords.y * (cos_arg * sin_inc);

	return (Coords3D) { .x = x, .y = y, .z = z };
}

double getDistanceFromCoords3D(Coords3D coords) {
	return sqrt((coords.x * coords.x) + (coords.y * coords.y) + (coords.z * coords.z));
}