CC = gcc
OPT_FLAGS = -O3 -march=native -masm=intel 
CFLAGS = $(OPT_FLAGS) -Wall -Wextra

TLE_DECODER_SRCS = main.c headers/OrbMaths.c headers/StaticPhase.c headers/TleFiles.c headers/TleParser.c headers/Conversions.c

ALL_SRCS = $(TLE_DECODER_SRCS)

compile:
	clear
	$(CC) $(CFLAGS) $(TLE_DECODER_SRCS) -flto -o newTLE.out -lm
	$(CC) $(CFLAGS) $(TLE_DECODER_SRCS) -S -lm