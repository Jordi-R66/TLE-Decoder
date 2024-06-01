#include "Common.h"

#include <unistd.h>

#ifdef _WIN32
	#define CLEAR_CMD "cls"
#elif defined(__linux__)
	#define CLEAR_CMD "clear"
#endif

void clear();