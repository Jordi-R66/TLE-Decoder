#include "Kepler.h"
#include "Ellipses.h"
#include "../ext_libs/matrix.h"

double keplerDistance(double a, double e, double E) {
	return a * (1.0 - e * cos(E));
}

double longitudeOfPeriapsis(double AN, double ArgPeri) {
	return AN + ArgPeri;
}

KeplerCoords2D_t sumCoords2D(KeplerCoords2D_t a, KeplerCoords2D_t b) {
	KeplerCoords2D_t coords;

	coords.x = a.x + b.x;
	coords.y = a.y + b.y;

	return coords;
}

KeplerCoords2D_t basic2DKeplerCoords(double a, double e, double E) {
	KeplerCoords2D_t coords;

	coords.x = a * (-cos(E) - e);
	coords.y = b(a, e) * -sin(E);

	//KeplerCoords2D_t focalPoint = FocalRelativeToBaricenter(a, e);
	coords = PointRelativeToBaricenter(baricenterRelativeToFocal(a, e), coords);

	return coords;
}

KeplerCoords2D_t coordsFromTA(double r, double TA) {
	KeplerCoords2D_t coords;

	coords.x = r * cos(TA);
	coords.y = r * sin(TA);

	return coords;
}

KeplerCoords2D_t Rotate2DKeplerCoords(KeplerCoords2D_t coords, double angleRot) {
	Matrix LongPeriRot, CoordsTemp, CoordsRot;

	LongPeriRot.rows = 2;
	LongPeriRot.cols = 2;

	CoordsTemp.rows = 2;
	CoordsTemp.cols = 1;

	CoordsRot.rows = 2;
	CoordsRot.cols = 1;

	allocMatrix(&LongPeriRot);
	allocMatrix(&CoordsTemp);
	allocMatrix(&CoordsRot);

	LongPeriRot.data[0] = cos(angleRot);
	LongPeriRot.data[1] = -sin(angleRot);
	LongPeriRot.data[2] = sin(angleRot);
	LongPeriRot.data[3] = cos(angleRot);

	CoordsTemp.data[0] = coords.x;
	CoordsTemp.data[1] = coords.y;

	matrixMultiplication(&LongPeriRot, &CoordsTemp, &CoordsRot);

	KeplerCoords2D_t RotatedCoords = *(KeplerCoords2D_t*)CoordsRot.data;

	deallocMatrix(&LongPeriRot);
	deallocMatrix(&CoordsTemp);
	deallocMatrix(&CoordsRot);

	return RotatedCoords;
}

KeplerCoords2D_t baricenterRelativeToFocal(double a, double e) {
	KeplerCoords2D_t coords;

	coords.x = -c(a, e);
	coords.y = 0.0;

	return coords;
}

KeplerCoords2D_t FocalRelativeToBaricenter(double a, double e) {
	KeplerCoords2D_t coords;

	coords.x = c(a, e);
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
