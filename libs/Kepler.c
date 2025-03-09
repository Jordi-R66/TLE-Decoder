#include "Kepler.h"
#include "Ellipses.h"
#include "../ext_libs/matrix.h"

double keplerDistance(double a, double e, double E) {
	return a * (1.0 - e * cos(E));
}

double longitudeOfPeriapsis(double AscNodeLong, double ArgPeri) {
	return AscNodeLong + ArgPeri;
}

KeplerCoords2D_t sumCoords2D(KeplerCoords2D_t a, KeplerCoords2D_t b) {
	KeplerCoords2D_t coords;

	coords.x = a.x + b.x;
	coords.y = a.y + b.y;

	return coords;
}

KeplerCoords2D_t subCoords2D(KeplerCoords2D_t a, KeplerCoords2D_t b) {
	KeplerCoords2D_t coords;

	coords.x = a.x - b.x;
	coords.y = a.y - b.y;

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

// -------------------------------------------------------------------------------------------------------------

KeplerCoords3D_t Rotate3DCoordsAroundAxis(KeplerCoords2D_t focal, KeplerCoords2D_t AscNode2D, KeplerCoords2D_t coords2D, double Inclination) {
	KeplerCoords3D_t coords3D = { coords2D.x, coords2D.y, 0.0 };

	AscNode2D = sumCoords2D(AscNode2D, focal);
	double AscNodeR = sqrt(pow(AscNode2D.x, 2) + pow(AscNode2D.y, 2));

	KeplerVector3D_t unitVector = { AscNode2D.x / AscNodeR, AscNode2D.y / AscNodeR, 0.0 };

	Matrix I, AntiSymUnitMatrix, ResultMatrix;

	I.rows = 3;
	I.cols = 3;

	AntiSymUnitMatrix.rows = 3;
	AntiSymUnitMatrix.cols = 3;

	allocMatrix(&AntiSymUnitMatrix);

	genIdentityMatrix(&I, 3);

	setMatrixCase(&AntiSymUnitMatrix, -unitVector.z, 0, 1);
	setMatrixCase(&AntiSymUnitMatrix, unitVector.y, 0, 2);
	setMatrixCase(&AntiSymUnitMatrix, unitVector.z, 1, 0);
	setMatrixCase(&AntiSymUnitMatrix, -unitVector.x, 1, 2);
	setMatrixCase(&AntiSymUnitMatrix, -unitVector.y, 2, 0);
	setMatrixCase(&AntiSymUnitMatrix, unitVector.x, 2, 1);

	Matrix uSqr;
	matrixMultiplication(&AntiSymUnitMatrix, &AntiSymUnitMatrix, &uSqr);
	Matrix sin_u = scalarMulNewMatrix(&AntiSymUnitMatrix, sin(Inclination));
	Matrix cos_u = scalarMulNewMatrix(&uSqr, 1.0 - cos(Inclination));

	Matrix I_sinU = matrixAdditionNewMatrix(&I, &sin_u);
	Matrix RotMatrix = matrixAdditionNewMatrix(&I_sinU, &cos_u);

	allocMatrix(&AntiSymUnitMatrix);

	Matrix coords3DMatrix;
	coords3DMatrix.rows = 3;
	coords3DMatrix.cols = 1;
	allocMatrix(&coords3DMatrix);

	coords3DMatrix.data[0] = coords3D.x;
	coords3DMatrix.data[1] = coords3D.y;
	coords3DMatrix.data[2] = coords3D.z;

	matrixMultiplication(&RotMatrix, &coords3DMatrix, &ResultMatrix);

	coords3D = *(KeplerCoords3D_t*)ResultMatrix.data;

	deallocMatrix(&uSqr);
	deallocMatrix(&sin_u);
	deallocMatrix(&cos_u);
	deallocMatrix(&I_sinU);
	deallocMatrix(&I);
	deallocMatrix(&RotMatrix);
	deallocMatrix(&AntiSymUnitMatrix);
	deallocMatrix(&ResultMatrix);
	deallocMatrix(&coords3DMatrix);

	return coords3D;
}
