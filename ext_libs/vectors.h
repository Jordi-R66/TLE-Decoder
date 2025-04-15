#pragma once

#include "matrix.h"

typedef Matrix Vector;

void allocVector(Vector* vector, size_t coords);
void deallocVector(Vector* vector);

void getCoord(Vector* vector, size_t coordNumber);
void setCoord(Vector* vector, size_t coordNumber, double value);

Vector crossProduct(Vector* vectorA, Vector* vectorB);
