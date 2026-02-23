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

void clear_screen(void) {
	#if defined(_WIN32)
		system("cls");   // Commande pour Windows
	#else
		system("clear"); // Commande pour Linux/macOS
	#endif
}