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
	system("cls");   // Commande pour Windows
#else
	system("clear"); // Commande pour Linux/macOS
#endif
}