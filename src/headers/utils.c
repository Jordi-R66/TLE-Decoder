#include "utils.h"

void sleep_ms(unsigned int milliseconds) {
	#ifdef _WIN32
		Sleep(milliseconds); // Windows prend des millisecondes directement
	#else
		struct timespec ts;
		ts.tv_sec = milliseconds / 1000;
		ts.tv_nsec = (milliseconds % 1000) * 1000000;
		nanosleep(&ts, NULL); // POSIX prend des secondes et nanosecondes
	#endif
}

void sleep_ns(uint64_t nanoseconds) {
#ifdef _WIN32
	// Sous Windows, on utilise un WaitableTimer pour des intervalles de 100 ns.
	// Une valeur négative indique un temps relatif (à partir de maintenant).
	HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (timer) {
		LARGE_INTEGER li;
		li.QuadPart = -(LONGLONG)(nanoseconds / 100);
		SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE);
		WaitForSingleObject(timer, INFINITE);
		CloseHandle(timer);
	}
#else
	struct timespec ts;
	ts.tv_sec = nanoseconds / 1000000000ULL;
	ts.tv_nsec = nanoseconds % 1000000000ULL;
	nanosleep(&ts, NULL);
#endif
}

void sleep_hz(unsigned int hz) {
	if (hz == 0) return; // Sécurité contre la division par zéro

	// 1 seconde = 1 000 000 000 nanosecondes
	uint64_t ns_per_frame = 1000000000ULL / hz;

	sleep_ns(ns_per_frame);
}

void clear_screen(void) {
	#if defined(_WIN32)
		static int last_width = 0;
		static int last_height = 0;

		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		// Récupère les infos actuelles de la console
		if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

		// Calcule la largeur et la hauteur de la fenêtre VISIBLE
		int current_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int current_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		// 1. Détection du redimensionnement
		if (current_width != last_width || current_height != last_height) {
			// La taille a changé ! On fait un VRAI clear instantané de l'API Windows.
			DWORD count;
			DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y; // Taille totale du buffer
			COORD homeCoords = { 0, 0 };

			// Remplit tout le buffer avec des espaces
			FillConsoleOutputCharacter(hOut, (TCHAR)' ', cellCount, homeCoords, &count);
			// Réinitialise les couleurs/attributs
			FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, homeCoords, &count);

			// Met à jour les dimensions connues
			last_width = current_width;
			last_height = current_height;
		}

		// 2. On place le curseur en haut à gauche de la fenêtre VISIBLE 
		// (et non pas au {0,0} absolu qui pourrait être caché dans l'historique)
		COORD pos = { csbi.srWindow.Left, csbi.srWindow.Top };
		SetConsoleCursorPosition(hOut, pos);
	#else
		system("clear"); // Commande pour Linux/macOS
#endif
}