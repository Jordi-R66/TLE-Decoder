#include "Referential.h"

#include "../headers/OrbMaths.h"

void Compute2DCoords(Coords2D* Coords, double OrbAlt, double TrueAnomaly) {
	if ((Coords->rows == 2) && (Coords->cols == 1) && (Coords->size == 2)) {
		// Computing X and Y coordinates on the orbit's plan
		Coords->data[0] = OrbAlt * cos(TrueAnomaly);
		Coords->data[1] = OrbAlt * sin(TrueAnomaly);

		//printf("2D Coordinates computed\n");
	} else {
		fprintf(stderr ,"Can't compute 2D Coordinates as the Coords Matrix isn't properly dimensione\nd\n%lu rows\n%lu cols\n%lu elements", Coords->rows, Coords->cols, Coords->size);
		exit(EXIT_FAILURE);
	}
}

void Compute2DSpeedVector(Vector2D* Vector, double SMA, double Eccentricity, double TrueAnomaly) {
	if ((Vector->rows == 2) && (Vector->cols == 1) && (Vector->size == 2)) {
		// Computing X and Y coordinates on the orbit's plan for the speed vector
		Vector->data[0] = -sqrt(EARTH_MU / (SMA * (1.0 - pow(Eccentricity, 2.0))) * sin(TrueAnomaly));
		Vector->data[1] = sqrt(EARTH_MU / (SMA * (1.0 - pow(Eccentricity, 2.0))) * (Eccentricity + cos(TrueAnomaly)));
	
		//printf("2D Vector computed\n");
	} else {
		perror("Can't compute 2D Vector as the Vector Matrix isn't properly dimensioned\n");
		exit(EXIT_FAILURE);
	}
}

void GenRotMatrices(Matrix* ArgPeriRotMat, Matrix* IncliRotMat, Matrix* ANRotMat, double ArgPeri, double Inclination, double AN) {
	bool RunningCondition;

	RunningCondition = (ArgPeriRotMat->cols == 3) && (ArgPeriRotMat->rows == 3) && (ArgPeriRotMat->size == 9);
	RunningCondition &= (IncliRotMat->cols == 3) && (IncliRotMat->rows == 3) && (IncliRotMat->size == 9);
	RunningCondition &= (ANRotMat->cols == 3) && (ANRotMat->rows == 3) && (ANRotMat->size == 9);

	if (RunningCondition) {
		for (size_t i = 0; i < 9; i++) {
			ArgPeriRotMat->data[i] = 0;
			IncliRotMat->data[i] = 0;
			ANRotMat->data[i] = 0;
		}

		ArgPeriRotMat->data[0] = cos(ArgPeri);
		ArgPeriRotMat->data[1] = -sin(ArgPeri);
		ArgPeriRotMat->data[3] = sin(ArgPeri);
		ArgPeriRotMat->data[4] = cos(ArgPeri);
		ArgPeriRotMat->data[8] = 1.0;

		IncliRotMat->data[0] = 1.0;
		IncliRotMat->data[4] = cos(Inclination);
		IncliRotMat->data[5] = -sin(Inclination);
		IncliRotMat->data[7] = sin(Inclination);
		IncliRotMat->data[8] = cos(Inclination);;

		ANRotMat->data[0] = cos(AN);
		ANRotMat->data[1] = -sin(AN);
		ANRotMat->data[3] = sin(AN);
		ANRotMat->data[4] = cos(AN);
		ANRotMat->data[8] = 1.0;

		//printf("Rotation Matrices computed\n");
	} else {
		perror("Can't compute Rotation Matrices as at least one Matrix isn't properly dimensioned\n");
		exit(EXIT_FAILURE);
	}
}

void RotateCoords(Matrix* ArgPeriRotMat, Matrix* IncliRotMat, Matrix* ANRotMat, Coords2D* InitCoords, Coords3D* RefCoords) {
	bool MatricesOk;
	bool CoordsOk;

	bool RunningCondition;

	MatricesOk = (ArgPeriRotMat->cols == 3) && (ArgPeriRotMat->rows == 3) && (ArgPeriRotMat->size == 9);
	MatricesOk &= (IncliRotMat->cols == 3) && (IncliRotMat->rows == 3) && (IncliRotMat->size == 9);
	MatricesOk &= (ANRotMat->cols == 3) && (ANRotMat->rows == 3) && (ANRotMat->size == 9);

	CoordsOk = (InitCoords->cols == 1) && (InitCoords->rows == 2) && (InitCoords->size == 2);
	CoordsOk &= (RefCoords->cols == 1) && (RefCoords->rows == 3) && (RefCoords->size == 3);

	RunningCondition = MatricesOk && CoordsOk;

	if (RunningCondition) {
		Coords3D Init3D;
		Init3D.rows = 3;
		Init3D.cols = 1;

		Coords3D TempRot;
		TempRot.rows = 3;
		TempRot.cols = 1;

		allocMatrix(&Init3D);
		allocMatrix(&TempRot);

		for (size_t i = 0; i < Init3D.size; i++) {
			Init3D.data[i] = 0.0;
		}

		for (size_t i = 0; i < InitCoords->size; i++) {
			Init3D.data[i] = InitCoords->data[i];
		}

		matrixMultiplication(ANRotMat, IncliRotMat, RefCoords);
		matrixMultiplication(RefCoords, ArgPeriRotMat, &TempRot);
		matrixMultiplication(&TempRot, &Init3D, RefCoords);

		deallocMatrix(&Init3D);
		deallocMatrix(&TempRot);

		//printf("Coordinates rotation computed\n");
	} else {
		fprintf(stderr, "Can't Rotate coords as at least one Matrix or Coord isn't properly dimensioned\n\nMatrices properly dimensioned : %c\nCoords properly dimensioned : %c\n", MatricesOk ? 'Y' : 'N', CoordsOk ? 'Y' : 'N');
		exit(EXIT_FAILURE);
	}
}

void RotateVector(Matrix* ArgPeriRotMat, Matrix* IncliRotMat, Matrix* ANRotMat, Vector2D* InitVector, Vector3D* RefVector) {
	bool MatricesOk;
	bool VectorsOk;

	bool RunningCondition;

	MatricesOk = (ArgPeriRotMat->cols == 3) && (ArgPeriRotMat->rows == 3) && (ArgPeriRotMat->size == 9);
	MatricesOk &= (IncliRotMat->cols == 3) && (IncliRotMat->rows == 3) && (IncliRotMat->size == 9);
	MatricesOk &= (ANRotMat->cols == 3) && (ANRotMat->rows == 3) && (ANRotMat->size == 9);

	VectorsOk = (InitVector->cols == 1) && (InitVector->rows == 2) && (InitVector->size == 2);
	VectorsOk &= (RefVector->cols == 1) && (RefVector->rows == 3) && (RefVector->size == 3);

	RunningCondition = MatricesOk && VectorsOk;

	if (RunningCondition) {
		Vector3D Init3D;
		Init3D.rows = 3;
		Init3D.cols = 1;

		Vector3D TempRot;
		TempRot.rows = 3;
		TempRot.cols = 1;

		allocMatrix(&Init3D);
		allocMatrix(&TempRot);

		for (size_t i = 0; i < Init3D.size; i++) {
			Init3D.data[i] = 0.0;
		}

		for (size_t i = 0; i < InitVector->size; i++) {
			Init3D.data[i] = InitVector->data[i];
		}

		matrixMultiplication(ANRotMat, IncliRotMat, RefVector);
		matrixMultiplication(RefVector, ArgPeriRotMat, &TempRot);
		matrixMultiplication(&TempRot, &Init3D, RefVector);

		deallocMatrix(&Init3D);
		deallocMatrix(&TempRot);

		//printf("Vector rotation computed\n");
	} else {
		fprintf(stderr, "Can't Rotate vectors as at least one Matrix or Vector isn't properly dimensioned\n\nMatrices properly dimensioned : %c\nVectors properly dimensioned : %c\n", MatricesOk ? 'Y' : 'N', VectorsOk ? 'Y' : 'N');
		exit(EXIT_FAILURE);
	}
}