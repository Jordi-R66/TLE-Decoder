#include "Geography.h"
#include "Constants.h"
#include "Conversions.h"

GeoCoords getGeoCoords(Coords3D eciCoords, time_t timestamp) {
	GeoCoords geo = { 0 };

	// 1. Calcul du GMST (Greenwich Mean Sidereal Time)
	// Conversion du timestamp UNIX en Jours Juliens depuis l'époque J2000.0
	double days_since_j2000 = ((double)timestamp / 86400.0) + 2440587.5 - 2451545.0;

	// Formule de l'USNO pour le GMST en degrés
	double gmst_deg = fmod(280.46061837 + 360.98564736629 * days_since_j2000, 360.0);
	if (gmst_deg < 0) gmst_deg += 360.0;
	double gmst_rad = gmst_deg * DEGS2RADS;

	// 2. LONGITUDE
	// Ascension droite du satellite dans le plan équatorial ECI
	double ra = atan2(eciCoords.y, eciCoords.x);

	// Longitude = Ascension Droite - Rotation de la Terre (GMST)
	double lon = ra - gmst_rad;

	// Normalisation de la longitude entre -PI et +PI (-180° à +180°)
	lon = fmod(lon + M_PI, 2.0 * M_PI);
	if (lon < 0) lon += 2.0 * M_PI;
	lon -= M_PI;

	geo.longitude = lon * RADS2DEGS;

	// 3. LATITUDE GÉODÉSIQUE (Méthode de Bowring itérative pour WGS84)
	double p = sqrt((eciCoords.x * eciCoords.x) + (eciCoords.y * eciCoords.y));

	// e² = 1 - (b²/a²) pour l'ellipsoïde WGS84
	// (WGS84_A et WGS84_B doivent être définis dans Constants.h)
	double e2 = 1.0 - ((WGS84_B * WGS84_B) / (WGS84_A * WGS84_A));

	double lat = atan2(eciCoords.z, p * (1.0 - e2));
	double N = 0.0;

	// Boucle de convergence (4 itérations pour une précision millimétrique)
	for (int i = 0; i < 4; i++) {
		N = WGS84_A / sqrt(1.0 - e2 * sin(lat) * sin(lat));
		lat = atan2(eciCoords.z + N * e2 * sin(lat), p);
	}

	geo.latitude = lat * RADS2DEGS;

	// 4. ALTITUDE GÉODÉSIQUE WGS84
	geo.altitude = (p / cos(lat)) - N;

	return geo;
}