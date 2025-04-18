#!/bin/bash
clear
COMPILER="gcc"

SOURCES="libs/TleFiles.c libs/OrbMaths.c libs/TleParser.c libs/CONVERSIONS.c libs/Algos.c libs/TermFuncs.c libs/Kepler.c libs/Ellipses.c ext_libs/myOwnCLib/matrices/matrix.c ext_libs/myOwnCLib/vectors/vectors.c main.c"

OUTPUT_DIR="output"

mkdir $OUTPUT_DIR

EXEC="TLE.exe"

ARGS="-Wall -Wno-unused-variable -O3 -std=c17 -m64 -march=x86-64 -flto"

$COMPILER $ARGS $SOURCES -o $OUTPUT_DIR/$EXEC -lm