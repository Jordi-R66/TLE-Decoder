#include "Kepler.h"
#include "Ellipses.h"

double keplerDistance(double a, double e, double E) {
	return a * (1.0 - e * cos(E));
}

double longitudeOfPeriapsis(double AscNodeLong, double ArgPeri) {
	return AscNodeLong + ArgPeri;
}

double longitudeToTA(double longitude, double LongPeri) {
	return LongPeri - longitude;
}

KeplerCoords2D_t baricenterRelativeToFocal(double a, double e) {
	KeplerCoords2D_t coords;

	coords.x = -c(a, e);
	coords.y = 0.0;

	return coords;
}

KeplerCoords2D_t PointRelativeToBaricenter(KeplerCoords2D_t baricenterPoint, KeplerCoords2D_t point) {
	KeplerCoords2D_t coords;

	coords.x = point.x - baricenterPoint.x;
	coords.y = point.y - baricenterPoint.y;

	return coords;
}

KeplerCoords2D_t PointRelativeToFocal(KeplerCoords2D_t focalPoint, KeplerCoords2D_t point) {
	KeplerCoords2D_t coords;

	coords.x = point.x - focalPoint.x;
	coords.y = point.y - focalPoint.y;

	return coords;
}

void changeReferential2D(KeplerCoords2D_t coords, KeplerCoords2D_t newReferential, KeplerCoords2D_t* result) {
	result->x = coords.x - newReferential.x;
	result->y = coords.y - newReferential.y;
}

void changeReferential3D(KeplerCoords3D_t coords, KeplerCoords3D_t newReferential, KeplerCoords3D_t* result) {
	result->x = coords.x - newReferential.x;
	result->y = coords.y - newReferential.y;
	result->z = coords.z - newReferential.z;
}