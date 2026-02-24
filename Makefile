CC = gcc
OPT_FLAGS = -s -std=c17 -O3 -march=native -masm=intel 
CFLAGS = $(OPT_FLAGS) -Wall -Wextra

LIBS_DIR=libs
CALENDAR_DIR=$(LIBS_DIR)/Calendar

TLE_DECODER_SRCS = main.c headers/*.c
CALENDAR_SRCS = $(CALENDAR_DIR)/Converters.c

ALL_SRCS = $(TLE_DECODER_SRCS) $(CALENDAR_SRCS)

all: clean compile assemble

compile:
	clear
	mkdir -p build
	$(CC) $(CFLAGS) $(ALL_SRCS) -flto -o build/TLE-Decoder -lm

assemble:
	mkdir -p asm
	$(CC) $(CFLAGS) $(ALL_SRCS) -S -lm
	mv *.s asm/

clean:
	rm asm/*.s