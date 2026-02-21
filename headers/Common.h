#pragma once

// Active les constantes mathématiques (M_PI) de <math.h> sous Windows (MSVC)
#define _USE_MATH_DEFINES 
// Active les extensions POSIX et constantes mathématiques sous Linux (glibc)
#define _GNU_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bool.h"

#ifdef __linux__
	#include <unistd.h>
#endif

typedef char* string;