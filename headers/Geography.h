#pragma once

#include "Common.h"
#include "Coords3D.h"
#include "TimeFuncs.h"

// Coordonnées Géographiques (Ellipsoïde WGS84)
typedef struct geo_coords {
	double latitude;  // En degrés [-90, 90]
	double longitude; // En degrés [-180, 180]
	double altitude;  // En mètres (au-dessus de l'ellipsoïde)
} GeoCoords;

/**
 * @brief Convertit des coordonnées ECI (Earth-Centered Inertial) en coordonnées géographiques (WGS84)
 *
 * @param eciCoords Les coordonnées cartésiennes 3D du satellite
 * @param timestamp Le timestamp UNIX actuel (pour calculer la rotation de la Terre)
 * @return GeoCoords Les coordonnées GPS (Latitude, Longitude, Altitude)
 */
GeoCoords getGeoCoords(Coords3D eciCoords, time_t timestamp);