#!/bin/bash
clear
COMPILER="gcc"

SOURCES="headers/TleFiles.c headers/OrbMaths.c headers/TleParser.c headers/Conversions.c headers/Algos.c headers/TermFuncs.c libs/myOwnCLib/matrices/matrix.c libs/myOwnCLib/vectors/vectors.c headers/Kepler.c headers/Ellipses.c headers/Intermediate.c headers/TimeFuncs.c main.c"

OUTPUT_DIR="output"

mkdir $OUTPUT_DIR

EXEC="TLE-Decoder.elf"

ARGS="-Wall -s -O3 -std=gnu17 -m64 -flto -ffunction-sections -fdata-sections -Wl,--gc-sections"
#ARGS="-Wall -Wno-unused-variable -O3 -std=c17 -m64 -march=x86-64 -flto"

$COMPILER $ARGS $SOURCES -o $OUTPUT_DIR/$EXEC -lm