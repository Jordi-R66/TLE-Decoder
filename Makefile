CC = gcc
OPT_FLAGS = -O3 -march=native -flto
CFLAGS = $(OPT_FLAGS) -Wall -Wextra

TLE_DECODER_SRCS = main.c headers/TleFiles.c headers/Conversions.c

ALL_SRCS = $(TLE_DECODER_SRCS)

compile:
	$(CC) $(CFLAGS) $(TLE_DECODER_SRCS) -o newTLE.out
	$(CC) $(CFLAGS) $(TLE_DECODER_SRCS) -S