#!/bin/bash
clear
COMPILER="gcc"

SOURCES="headers/TleFiles.c headers/OrbMaths.c headers/TleParser.c headers/CONVERSIONS.c headers/Algos.c headers/TermFuncs.c headers/Kepler.c headers/Ellipses.c libs/myOwnCLib/matrices/matrix.c libs/myOwnCLib/vectors/vectors.c main.c"

OUTPUT_DIR="output"

mkdir $OUTPUT_DIR

EXEC="TLE.elf"

ARGS="-Wall -g -O0 -std=c23"
#ARGS="-Wall -Wno-unused-variable -O3 -std=c17 -m64 -march=x86-64 -flto"

$COMPILER $ARGS $SOURCES -o $OUTPUT_DIR/$EXEC -lm