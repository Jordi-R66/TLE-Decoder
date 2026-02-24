#include "Coords3D.h"
#include "OrbMaths.h"

Coords3D getECICoords(Coords2D planeCoords, TLE* tle) {
	double an = (double)tle->AscNodeLong * DEGS2RADS;
	double arg = (double)tle->PeriArg * DEGS2RADS;
	double inc = (double)tle->Inclination * DEGS2RADS;

	double cos_an = cos(an);
	double sin_an = sin(an);
	double cos_arg = cos(arg);
	double sin_arg = sin(arg);
	double cos_inc = cos(inc);
	double sin_inc = sin(inc);

	double x, y, z;

	x = planeCoords.x * (cos_an * cos_arg - sin_an * sin_arg * cos_inc) + planeCoords.y * (-cos_an * sin_arg - sin_an * cos_arg * cos_inc);
	y = planeCoords.x * (sin_an * cos_arg + cos_an * sin_arg * cos_inc) + planeCoords.y * (-sin_an * sin_arg + cos_an * cos_arg * cos_inc);
	z = planeCoords.x * (sin_arg * sin_inc) + planeCoords.y * (cos_arg * sin_inc);

	return (Coords3D) { .x = x, .y = y, .z = z };
}

Coords3D getECICoordsFromTA(double nu, double a, TLE* tle) {
	double e = tle->Eccentricity;
	double r = AltFromTA(a, e, nu);
	Coords2D intermediaire = getPlaneCoords(nu, r);
	return getECICoords(intermediaire, tle);
}

double getDistanceFromCoords3D(Coords3D coords) {
	return sqrt((coords.x * coords.x) + (coords.y * coords.y) + (coords.z * coords.z));
}

double getWGS84Altitude(Coords3D coords) {
	// Distance géocentrique (du centre de la Terre au satellite)
	double r = getDistanceFromCoords3D(coords);

	if (r == 0.0) return 0.0; // Sécurité division par zéro

	// a = Rayon équatorial, b = Rayon polaire
	double a = WGS84_A;
	double b = WGS84_B;

	// Déduction de cos²(latitude) et sin²(latitude) directement depuis les coordonnées 3D
	double xy_sqr = (coords.x * coords.x) + (coords.y * coords.y);
	double z_sqr = (coords.z * coords.z);

	double cos2_phi = xy_sqr / (r * r);
	double sin2_phi = z_sqr / (r * r);

	// Calcul du rayon de l'ellipsoïde terrestre sous le satellite
	double a2 = a * a;
	double b2 = b * b;

	double num = (a2 * a2 * cos2_phi) + (b2 * b2 * sin2_phi);
	double den = (a2 * cos2_phi) + (b2 * sin2_phi);

	double earth_radius = sqrt(num / den);

	// Altitude WGS84 = Distance au centre - Rayon de la Terre local
	return r - earth_radius;
}

double getWGS84AltitudeFromTA(double nu, double a, TLE* tle) {
	Coords3D coords = getECICoordsFromTA(nu, a, tle);

	return getWGS84Altitude(coords);
}