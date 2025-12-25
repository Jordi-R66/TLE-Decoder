#pragma once

#define M_PI 3.14159265358979323846

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

typedef char string;
typedef char* cstring;