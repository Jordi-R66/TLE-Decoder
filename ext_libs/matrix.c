#include "matrix.h"

void allocMatrix(Matrix* matrix) {
	matrix->size = matrix->rows * matrix->cols;
	matrix->data = (double*)calloc(matrix->size, sizeof(double));
	matrix->memFreed = false;
}

void deallocMatrix(Matrix* matrix) {
	free(matrix->data);
	matrix->memFreed = true;
}

void getMatrixRow(Matrix* matrix, size_t row, double* rowBuffer) {
	size_t iStart, iEnd;

	iStart = row * matrix->cols;
	iEnd = (row + 1) * matrix->cols;

	for (size_t i = iStart; i < iEnd; i++) {
		rowBuffer[i - iStart] = matrix->data[i];
	}
}

void getMatrixColumn(Matrix* matrix, size_t column, double* colBuffer) {
	for (size_t row = 0; row < matrix->rows; row++) {
		size_t i = row * matrix->cols + column;
		colBuffer[row] = matrix->data[i];
	}
}

void setMatrixCase(Matrix* matrix, double value, size_t row, size_t col) {
	size_t i = row * matrix->cols + col;

	matrix->data[i] = value;
}

double getMatrixCase(Matrix* matrix, size_t row, size_t col) {
	size_t i = row * matrix->cols + col;

	return matrix->data[i];
}

void scalarMul(Matrix* matrix, double scalar) {
	for (size_t i = 0; i < matrix->size; i++) {
		matrix->data[i] *= scalar;
	}
}

/*
	Performs an inefficient matrix multiplication algorithm and stores the resulting matrix at a given address
	Stores the result `matDest` of `matA * matB`
*/
void matrixMultiplication(Matrix* matA, Matrix* matB, Matrix* matDest) {
	if (matA->cols != matB->rows) {
		return;
	}

	matDest->rows = matA->rows;
	matDest->cols = matB->cols;

	allocMatrix(matDest);

	double* col = (double*)calloc(matDest->rows, sizeof(double));
	double* row = (double*)calloc(matDest->cols, sizeof(double));

	for (size_t i = 0; i < matDest->rows; i++) {
		for (size_t j = 0; j < matDest->cols; j++) {
			double newValue = 0.0;

			getMatrixRow(matA, i, row);
			getMatrixColumn(matB, j, col);

			for (size_t k = 0; k < matB->cols; k++) {
				newValue += col[k] * row[k];
			}

			setMatrixCase(matDest, newValue, i, j);
		}
	}

	free(col);
	free(row);
}

/*
	Adds matB to matA and stores the result in matA
*/
void matrixAddition(Matrix* matA, Matrix* matB) {
	if ((matA->cols != matB->cols) || (matA->rows != matB->rows)) {
		return;
	}

	for (size_t i = 0; i < matA->size; i++) {
		matA->data[i] += matB->data[i];
	}
}

void genIdentityMatrix(Matrix* matrix, size_t n) {
	matrix->cols = n;
	matrix->rows = n;
	allocMatrix(matrix);
}

bool isInvertible(Matrix* matrix) {
	if (matrix->rows != matrix->cols) {
		return false;
	}

	size_t dim = matrix->cols;

	Matrix IdentityMatrix;
}
