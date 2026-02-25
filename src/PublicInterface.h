#pragma once

// Inclusion de tes headers internes nécessaires
#include "headers/TleParser.h"
#include "headers/StaticPhase.h"
#include "headers/DynamicPhase.h"
#include "headers/TimeFuncs.h"

// ============================================================================
// STRUCTURE GLOBALE
// ============================================================================

typedef struct {
	// Données de base du satellite
	TLE tle;

	// Paramètres orbitaux constants (calculés une seule fois)
	StaticValues statics;

	// Dernière position/vitesse calculée
	DynamicValues current;

	// Statut (ex: pour savoir si l'initialisation a réussi)
	bool is_initialized;

} SatelliteData, *SatData_ptr;

// ============================================================================
// FONCTIONS PRATIQUES (L'interface)
// ============================================================================

/**
 * @brief Initialise le suivi d'un satellite à partir d'un fichier TLE. Cette fonction lit le TLE, cherche le NORAD ID, et calcule la phase statique.
 * @param filename Le chemin vers le fichier .tle
 * @param noradId L'identifiant NORAD du satellite recherché
 * @param out_data Pointeur vers la structure globale à initialiser
 * @return true si le satellite a été trouvé et initialisé, false sinon.
 */
bool sattrack_init_from_file(const char* filename, uint32_t noradId, SatData_ptr out_data);

/**
 * @brief Met à jour la position et la vitesse du satellite pour un instant T.
 * @param data Pointeur vers la structure globale préalablement initialisée.
 * @param timestamp Le timestamp UNIX cible (utiliser time(NULL) pour l'instant présent).
 */
void sattrack_update(SatData_ptr data, time_t timestamp);