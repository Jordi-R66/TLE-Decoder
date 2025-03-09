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

/**
 * @brief Computes the distance between the Earth and the satellite using the orbital parameters
 * 
 * @param a Semi major axis
 * @param e Eccentricity
 * @param E Eccentric anomaly
 * @return The distance in meters
 */
double keplerDistance(double a, double e, double E);

/**
 * @brief Computes the longitude of the periapsis of an orbit
 * 
 * @param AscNodeLong Longitude of the ascending node (radians)
 * @param ArgPeri Argument of periapsis (radians)
 * @return double The longitude of the periapsis in radians
 */
double longitudeOfPeriapsis(double AscNodeLong, double ArgPeri);

KeplerCoords2D_t sumCoords2D(KeplerCoords2D_t a, KeplerCoords2D_t b);

KeplerCoords2D_t subCoords2D(KeplerCoords2D_t a, KeplerCoords2D_t b);

/**
 * @brief Computes the 2D coordinates (relative to the baricenter) of a point on an orbit
 * 
 * @param a Semi major axis
 * @param e Eccentricity
 * @param E Eccentric anomaly
 * @return KeplerCoords2D_t The coordinates of the point relative to the baricenter
 */
KeplerCoords2D_t basic2DKeplerCoords(double a, double e, double E);

/**
 * @brief Returns the coordinates of a point in 2D space given its distance from the focal and its true anomaly
 * 
 * @param r Distance from the focal point
 * @param TA True anomaly in radians
 * @return KeplerCoords2D_t Coordinates of the point in 2D space relative to the focal point
 */
KeplerCoords2D_t coordsFromTA(double r, double TA);

/**
 * @brief Rotates a 2D point by a given angle in radians
 * 
 * @param coords Baricenter relative coordinates of the point relative to the baricenter
 * @param LongPeri Longitude of the periapsis in radians
 * @return KeplerCoords2D_t The new coordinates of the point relative to the baricenter
 */
KeplerCoords2D_t Rotate2DKeplerCoords(KeplerCoords2D_t coords, double angleRot);

/**
 * @brief Computes the coordinates of the baricenter relative to the focal point of an orbit
 * 
 * @param a Semi major axis
 * @param e Eccentricity
 * @return KeplerCoords2D_t The coordinates of the baricenter relative to the focal point
 */
KeplerCoords2D_t baricenterRelativeToFocal(double a, double e);

/**
 * @brief Computes the coordinates of the focal point relative to the baricenter of an orbit
 * (Assumes no rotation has been done)
 * 
 * @param a Semi major axis
 * @param e Eccentricity
 * @return KeplerCoords2D_t 
 */
KeplerCoords2D_t FocalRelativeToBaricenter(double a, double e);

/**
 * @brief Computes the coordinates of a point relative to the baricenter of an orbit
 * 
 * @param baricenterPoint the coordinates of the baricenter relative to the focal point
 * @param point the coordinates of the point relative to the focal point
 * @return KeplerCoords2D_t the coordinates of the point relative to the baricenter
 */
KeplerCoords2D_t PointRelativeToBaricenter(KeplerCoords2D_t baricenterPoint, KeplerCoords2D_t point);

/**
 * @brief Computes the coordinates of a point relative to the focal point of an orbit
 * 
 * @param focalPoint the coordinates of the focal point relative to the baricenter
 * @param point the coordinates of the point relative to the baricenter
 * @return KeplerCoords2D_t the coordinates of the point relative to the focal point
 */
KeplerCoords2D_t PointRelativeToFocal(KeplerCoords2D_t focalPoint, KeplerCoords2D_t point);

/**
 * @brief Computes the coordinates of a point in 3D space after rotating it around an arbitrary axis
 * 
 * @param AxisPoint2D A point on the axis of rotation (abs coordinates)
 * @param coords2D The 2D coordinates of the point to rotate (abs coordinates)
 * @param Inclination The inclination of the orbit in radians
 * @return KeplerCoords3D_t The 3D coordinates of the point after rotation (abs coordinates)
 */
KeplerCoords3D_t Rotate3DCoordsAroundAxis(KeplerCoords2D_t AxisPoint2D, KeplerCoords2D_t coords2D, double Inclination);