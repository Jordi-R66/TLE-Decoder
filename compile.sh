#!/bin/bash
clear
COMPILER="gcc"

SOURCES="libs/TleFiles.c libs/OrbMaths.c libs/TleParser.c libs/CONVERSIONS.c libs/Algos.c libs/TermFuncs.c main.c"

OUTPUT_DIR="output"

mkdir $OUTPUT_DIR

EXEC="TLE.exe"

ARGS="-Wall -O3 -std=c17 -m64 -march=x86-64 -flto"

$COMPILER $ARGS $SOURCES -o $OUTPUT_DIR/$EXEC