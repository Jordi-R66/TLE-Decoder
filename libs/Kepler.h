#include "Common.h"

struct KeplerCoords2D {
	double x;
	double y;
};

struct KeplerCoords3D {
	double x;
	double y;
	double z;
};


typedef struct KeplerCoords2D KeplerCoords2D_t;
typedef struct KeplerCoords2D KeplerVector2D_t;

typedef struct KeplerCoords3D KeplerCoords3D_t;
typedef struct KeplerCoords3D KeplerVector3D_t;

double keplerDistance(double a, double e, double E);
KeplerCoords2D_t basic2DKeplerCoords(double a, double e, double E);
KeplerCoords2D_t ANRot2DKeplerCoords(KeplerCoords2D_t coords, double AN);