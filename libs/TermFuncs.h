#include "Common.h"

#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif

bool SupportsAnsi();
void ClearScreen();
void MoveTo();