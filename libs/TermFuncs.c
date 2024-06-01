#include "TermFuncs.h"

#ifdef _WIN32

bool SupportsAnsi() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		return false;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		return false;
	}

	return true;
}
#else
bool SupportsAnsi() {
	return true;
}
#endif

void ClearScreen() {
	if (SupportsAnsi()) {
		printf("\033[H\033[J");
	} else {
		system(CLEAR_CMD);
	}

	return;
}

void MoveTo(int x, int y) {
	if (SupportsAnsi()) {
		printf("\033[%d;%dH", y, x);
	}
}