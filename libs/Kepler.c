#include "Kepler.h"
#include "Ellipses.h"
#include "../ext_libs/matrix.h"

double keplerDistance(double a, double e, double E) {
	return a * (1.0 - e * cos(E));
}

KeplerCoords2D_t basic2DKeplerCoords(double a, double e, double E) {
	KeplerCoords2D_t coords;

	coords.x = a * (cos(E) - e);
	coords.y = b(a, e) * sin(E);

	return coords;
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