#pragma once

#include "Common.h"
#include "../ext_libs/myOwnCLib/matrices/matrix.h"

typedef Matrix Coords2D;
typedef Matrix Coords3D;

typedef Matrix Vector2D;
typedef Matrix Vector3D;

void Compute2DCoords(Coords2D* Coords, double OrbAlt, double TrueAnomaly);
void Compute2DSpeedVector(Vector2D* Vector, double SMA, double Eccentricity, double TrueAnomaly);
void GenRotMatrices(Matrix* ArgPeriRotMat, Matrix* IncliRotMat, Matrix* ANRotMat, double ArgPeri, double Inclination, double AN);
void RotateCoords(Matrix* ArgPeriRotMat, Matrix* IncliRotMat, Matrix* ANRotMat, Coords2D* InitCoords, Coords3D* RefCoords);
void RotateVector(Matrix* ArgPeriRotMat, Matrix* IncliRotMat, Matrix* ANRotMat, Vector2D* InitVector, Vector3D* RefVector);
