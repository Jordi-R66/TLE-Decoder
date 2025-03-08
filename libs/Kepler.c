#include "Kepler.h"
#include "Ellipses.h"
#include "../ext_libs/matrix.h"

double keplerDistance(double a, double e, double E) {
	return a * (1.0 - e * cos(E));
}

KeplerCoords2D_t basic2DKeplerCoords(double a, double e, double E) {
	KeplerCoords2D_t coords;

	coords.x = a * (cos(E + M_PI / 2.0) - e);
	coords.y = b(a, e) * sin(E + M_PI / 2.0);

	KeplerCoords2D_t focalPoint = FocalRelativeToBaricenter(a, e);
	coords = PointRelativeToBaricenter(focalPoint, coords);

	return coords;
}

KeplerCoords2D_t RotatePeri2DKeplerCoords(KeplerCoords2D_t coords, double LongPeri) {
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

	LongPeriRot.data[0] = cos(LongPeri);
	LongPeriRot.data[1] = -sin(LongPeri);
	LongPeriRot.data[2] = sin(LongPeri);
	LongPeriRot.data[3] = cos(LongPeri);

	CoordsTemp.data[0] = coords.x;
	CoordsTemp.data[1] = coords.y;

	matrixMultiplication(&LongPeriRot, &CoordsTemp, &CoordsRot);

	KeplerCoords2D_t RotatedCoords = *(KeplerCoords2D_t*)CoordsRot.data;

	deallocMatrix(&LongPeriRot);
	deallocMatrix(&CoordsTemp);
	deallocMatrix(&CoordsRot);

	return RotatedCoords;
}

KeplerCoords2D_t ANRot2DKeplerCoords(KeplerCoords2D_t coords, double AN) {
	Matrix ANRotMat, CoordsTemp, CoordsRot;

	ANRotMat.rows = 2;
	ANRotMat.cols = 2;

	CoordsTemp.rows = 2;
	CoordsTemp.cols = 1;

	CoordsRot.rows = 2;
	CoordsRot.cols = 1;

	allocMatrix(&ANRotMat);
	allocMatrix(&CoordsTemp);
	allocMatrix(&CoordsRot);

	ANRotMat.data[0] = cos(AN);
	ANRotMat.data[1] = -sin(AN);
	ANRotMat.data[2] = sin(AN);
	ANRotMat.data[3] = cos(AN);

	CoordsTemp.data[0] = coords.x;
	CoordsTemp.data[1] = coords.y;

	matrixMultiplication(&ANRotMat, &CoordsTemp, &CoordsRot);

	KeplerCoords2D_t RotatedCoords = *(KeplerCoords2D_t*)CoordsRot.data;

	deallocMatrix(&ANRotMat);
	deallocMatrix(&CoordsTemp);
	deallocMatrix(&CoordsRot);

	return RotatedCoords;
}

KeplerCoords2D_t baricenterRelativeToFocal(double a, double e) {
	KeplerCoords2D_t coords;

	coords.x = c(a, e);
	coords.y = 0.0;

	return coords;
}

KeplerCoords2D_t FocalRelativeToBaricenter(double a, double e) {
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
