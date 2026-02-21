CC = gcc
OPT_FLAGS = -O3 -march=native -masm=intel 
CFLAGS = $(OPT_FLAGS) -Wall -Wextra

LIBS_DIR=libs
CALENDAR_DIR=$(LIBS_DIR)/Calendar

TLE_DECODER_SRCS = main.c headers/TimeFuncs.c headers/OrbMaths.c headers/StaticPhase.c headers/TleFiles.c headers/TleParser.c headers/Conversions.c
CALENDAR_SRCS = $(CALENDAR_DIR)/Converters.c

ALL_SRCS = $(TLE_DECODER_SRCS) $(CALENDAR_SRCS)

compile:
	clear
	$(CC) $(CFLAGS) $(ALL_SRCS) -flto -o newTLE.out -lm
	$(CC) $(CFLAGS) $(ALL_SRCS) -S -lm

clean:
	rm *.s