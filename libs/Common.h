#pragma once

#define M_PI 3.14159265358979323846

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef __linux__
	#include <unistd.h>
#endif

#if defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L
/* bool, true and false are keywords.  */
#else
#include <stdbool.h>
#endif

typedef char string;
typedef char* cstring;