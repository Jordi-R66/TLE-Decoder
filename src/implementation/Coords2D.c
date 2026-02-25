#include "../headers/Coords2D.h"

Coords2D getPlaneCoords(double nu, double r) {
	return (Coords2D) { .x = r * cos(nu), .y = r * sin(nu) };
}

double getDistanceFromCoords2D(Coords2D coords) {
	return sqrt( (coords.x * coords.x) + (coords.y * coords.y) );
}
