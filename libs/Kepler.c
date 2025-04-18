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

// ---------------------------------------------------------------------------------------

Vector coordsFromTA(double r, double TA) {
	Vector coords;

	allocVector(&coords, 2);

	setCoord(&coords, 0, r * cos(TA));
	setCoord(&coords, 1, r * sin(TA));

	return coords;
}

Vector Rotate2D(Vector* coords, value_t angleRot) {
	Matrix RotMat;
	RotMat.rows = 2;
	RotMat.cols = 2;

	allocMatrix(&RotMat);

	setMatrixCase(&RotMat, cos(angleRot), 0, 0);
	setMatrixCase(&RotMat, -sin(angleRot), 0, 1);
	setMatrixCase(&RotMat, sin(angleRot), 1, 0);
	setMatrixCase(&RotMat, cos(angleRot), 1, 1);

	Vector newCoords;

	matrixMultiplication(&RotMat, (Matrix*)coords, (Matrix*)&newCoords);

	return newCoords;
}

Vector unitVector2D(value_t x, value_t y) {
	Vector unitVec;
	allocVector(&unitVec, 2);

	value_t norm = sqrt(pow(x, 2) + pow(y, 2));

	if (norm == 0) {
		// Handle the error: cannot create a unit vector with null length
		fprintf(stderr, "Error: Cannot create a unit vector with null length.\n");
		deallocVector(&unitVec);
		exit(EXIT_FAILURE);
	}

	setCoord(&unitVec, 0, x / norm);
	setCoord(&unitVec, 1, y / norm);

	return unitVec;
}

Vector Rotate3D(Vector* unitVector, Vector* currentVector, value_t angleRot) {
	Vector rotatedVector;

	Vector crossProductVector;

	// Rodrigues' rotation formula
	// `v' = v * cos(θ) + (u × v) * sin(θ) + u * (u . v) * (1 - cos(θ))`
	// where u is the unit vector, v is the current vector, and θ is the rotation angle and v' is the rotated vector

	// Transpose the unit vector and the current Vector into 3D
	Vector unitVector3D, currentVector3D;
	allocVector(&currentVector3D, 3);
	allocVector(&unitVector3D, 3);

	memcpy(unitVector3D.data, unitVector->data, unitVector->size * sizeof(value_t));
	memcpy(currentVector3D.data, currentVector->data, currentVector->size * sizeof(value_t));

	// Set the z component to 0
	setCoord(&unitVector3D, 2, 0.0);
	setCoord(&currentVector3D, 2, 0.0);

	// Calculate `v * cos(θ)`
	rotatedVector = (Vector)scalarMulNewMatrix((Matrix*)&currentVector3D, cos(angleRot));

	// Calculate `(u × v) * sin(θ)`
	crossProductVector = crossProduct(&unitVector3D, &currentVector3D);
	scalarMul(&crossProductVector, sin(angleRot));

	// Calculate `u * (u . v) * (1 - cos(θ))`
	value_t dot_product = dotProduct(&unitVector3D, &currentVector3D);
	Vector Vec3 = scalarMulNewMatrix((Matrix*)&unitVector3D, dot_product);
	scalarMul(&Vec3, 1.0 - cos(angleRot));	

	// Calculate the rotated vector
	matrixAddition(&rotatedVector, (Matrix*)&crossProductVector);
	matrixAddition(&rotatedVector, (Matrix*)&Vec3);

	// Free the allocated memory
	deallocVector(&unitVector3D);
	deallocVector(&currentVector3D);
	deallocVector(&crossProductVector);
	deallocVector(&Vec3);

	return rotatedVector;
}

/*
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
KeplerCoords3D_t Rotate3DCoordsAroundAxis(KeplerCoords2D_t AxisPoint2D, KeplerCoords2D_t coords2D, double Inclination) {
	KeplerCoords3D_t coords3D = { 0.0, 0.0, 0.0 };

	coords3D.x = coords2D.x;
	coords3D.y = coords2D.y;

	// 1️⃣ Calcul du vecteur unitaire de l'axe (FIXE)
	double AxisPointR = sqrt(pow(AxisPoint2D.x, 2) + pow(AxisPoint2D.y, 2));
	if (AxisPointR == 0) {
		printf("Erreur : Le point d'axe ne peut pas être à l'origine.\n");
		return coords3D;
	}

	KeplerVector3D_t unitVector = { AxisPoint2D.x / AxisPointR, AxisPoint2D.y / AxisPointR, 0.0 };
	printf("Vecteur unitaire : (%lf, %lf, %lf)\n", unitVector.x, unitVector.y, unitVector.z);

	return coords3D;

	// 2️⃣ Matrice antisymétrique [u]×
	Matrix u;
	u.rows = 3;
	u.cols = 3;
	allocMatrix(&u);

	setMatrixCase(&u, 0.0, 0, 0);
	setMatrixCase(&u, -unitVector.z, 0, 1);
	setMatrixCase(&u, unitVector.y, 0, 2);
	setMatrixCase(&u, unitVector.z, 1, 0);
	setMatrixCase(&u, 0.0, 1, 1);
	setMatrixCase(&u, -unitVector.x, 1, 2);
	setMatrixCase(&u, -unitVector.y, 2, 0);
	setMatrixCase(&u, unitVector.x, 2, 1);
	setMatrixCase(&u, 0.0, 2, 2);

	// 3️⃣ Matrice de rotation R = I + sin(θ) [u]× + (1 - cos(θ)) [u]×²
	Matrix I, uSqr, sin_u, R;
	genIdentityMatrix(&I, 3);          // Matrice identité
	matrixMultiplication(&u, &u, &uSqr); // u²
	sin_u = scalarMulNewMatrix(&u, sin(Inclination));
	scalarMul(&uSqr, 1.0 - cos(Inclination));

	allocMatrix(&R);
	matrixAddition(&R, &I);
	matrixAddition(&R, &sin_u);
	matrixAddition(&R, &uSqr);

	// 4️⃣ Transformation du point coords2D en 3D
	Matrix coords3DMatrix;
	coords3DMatrix.rows = 3;
	coords3DMatrix.cols = 1;
	allocMatrix(&coords3DMatrix);

	setMatrixCase(&coords3DMatrix, coords2D.x, 0, 0);
	setMatrixCase(&coords3DMatrix, coords2D.y, 1, 0);
	setMatrixCase(&coords3DMatrix, 0.0, 2, 0);  // Plan 2D → 3D

	// 5️⃣ Appliquer la rotation
	Matrix ResultMatrix;
	allocMatrix(&ResultMatrix);
	matrixMultiplication(&R, &coords3DMatrix, &ResultMatrix);

	// 6️⃣ Extraction des nouvelles coordonnées
	coords3D.x = getMatrixCase(&ResultMatrix, 0, 0);
	coords3D.y = getMatrixCase(&ResultMatrix, 1, 0);
	coords3D.z = getMatrixCase(&ResultMatrix, 2, 0);

	// 7️⃣ Libération de la mémoire
	deallocMatrix(&u);
	deallocMatrix(&uSqr);
	deallocMatrix(&sin_u);
	deallocMatrix(&I);
	deallocMatrix(&R);
	deallocMatrix(&coords3DMatrix);
	deallocMatrix(&ResultMatrix);

	return coords3D;
}

/*
KeplerCoords3D_t Rotate3DCoordsAroundAxis(KeplerCoords2D_t AxisPoint2D, KeplerCoords2D_t coords2D, double Inclination) {
	KeplerCoords3D_t coords3D = { 0.0, 0.0, 0.0 };

	double AxisPointR = sqrt(pow(AxisPoint2D.x, 2) + pow(AxisPoint2D.y, 2));
	KeplerVector3D_t unitVector = { AxisPoint2D.x / AxisPointR, AxisPoint2D.y / AxisPointR, 0.0 };

	Matrix u;

	u.rows = 3;
	u.cols = 3;

	allocMatrix(&u);

	setMatrixCase(&u, -unitVector.z, 0, 1);
	setMatrixCase(&u, unitVector.y, 0, 2);
	setMatrixCase(&u, unitVector.z, 1, 0);
	setMatrixCase(&u, -unitVector.x, 1, 2);
	setMatrixCase(&u, -unitVector.y, 2, 0);
	setMatrixCase(&u, unitVector.x, 2, 1);

	Matrix uSqr;

	matrixMultiplication(&u, &u, &uSqr);

	Matrix sin_u = scalarMulNewMatrix(&u, sin(Inclination));

	scalarMul(&uSqr, 1.0 - cos(Inclination));

	Matrix I;

	genIdentityMatrix(&I, 3);

	Matrix R;

	R.cols = 3;
	R.rows = 3;

	allocMatrix(&R);

	matrixAddition(&R, &I);
	matrixAddition(&R, &sin_u);
	matrixAddition(&R, &uSqr);

	Matrix coords3DMatrix;

	coords3DMatrix.rows = 3;
	coords3DMatrix.cols = 1;

	allocMatrix(&coords3DMatrix);

	coords3DMatrix.data[0] = coords2D.x;
	coords3DMatrix.data[1] = coords2D.y;
	coords3DMatrix.data[2] = 0.0;

	Matrix ResultMatrix;

	matrixMultiplication(&R, &coords3DMatrix, &ResultMatrix);

	coords3D.x = getMatrixCase(&ResultMatrix, 0, 0);
	coords3D.y = getMatrixCase(&ResultMatrix, 1, 0);
	coords3D.z = getMatrixCase(&ResultMatrix, 2, 0);

	deallocMatrix(&u);
	deallocMatrix(&uSqr);
	deallocMatrix(&sin_u);
	deallocMatrix(&I);
	deallocMatrix(&R);
	deallocMatrix(&coords3DMatrix);
	deallocMatrix(&ResultMatrix);

	return coords3D;
}*/

/*
KeplerCoords3D_t Rotate3DCoordsAroundAxis(KeplerCoords2D_t AxisPoint2D, KeplerCoords2D_t coords2D, double Inclination) {
	KeplerCoords3D_t coords3D = { coords2D.x, coords2D.y, 0.0 };

	double AxisPointR = sqrt(pow(AxisPoint2D.x, 2) + pow(AxisPoint2D.y, 2));

	KeplerVector3D_t unitVector = { AxisPoint2D.x / AxisPointR, AxisPoint2D.y / AxisPointR, 0.0 };

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
}*/

void changeReferential2D(KeplerCoords2D_t coords, KeplerCoords2D_t newReferential, KeplerCoords2D_t* result) {
	result->x = coords.x - newReferential.x;
	result->y = coords.y - newReferential.y;
}

void changeReferential3D(KeplerCoords3D_t coords, KeplerCoords3D_t newReferential, KeplerCoords3D_t* result) {
	result->x = coords.x - newReferential.x;
	result->y = coords.y - newReferential.y;
	result->z = coords.z - newReferential.z;
}