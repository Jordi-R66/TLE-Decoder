#!/bin/bash
clear
COMPILER="gcc"

SOURCES="headers/TleFiles.c headers/OrbMaths.c headers/TleParser.c headers/Conversions.c headers/Intermediate.c headers/Kepler.c headers/TimeFuncs.c headers/Algos.c headers/TermFuncs.c headers/Ellipses.c main.c"

OUTPUT_DIR="output"

mkdir -p $OUTPUT_DIR

EXEC="TLE.elf"

#ARGS="-Wall -Wextra --fast-math -s -Os -std=c23 -masm=intel"
ARGS="-Wall -Wextra -Wno-unused-variable -s -O3 -std=c17 -m64 -march=x86-64 -flto -masm=intel"

$COMPILER $ARGS $SOURCES -o $OUTPUT_DIR/$EXEC -lm
$COMPILER $ARGS $SOURCES -S