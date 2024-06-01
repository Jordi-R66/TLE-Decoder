#include "Common.h"

#include <unistd.h>

#ifdef _WIN32
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif

void ClearScreen();