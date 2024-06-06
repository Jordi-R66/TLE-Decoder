@echo off
cls
set COMPILER=gcc.exe

set SOURCES=libs/TLE_Files.c libs/OrbMaths.c libs/TleParser.c libs/CONVERSIONS.c libs/Algos.c libs/TermFuncs.c main.c

set OUTPUT_DIR=output/

mkdir %OUTPUT_DIR%

set EXEC=TLE.exe

set ARGS=-O3 -std=c17 -m64 -march=x86-64 -flto

%COMPILER% %ARGS% %SOURCES% -o %OUTPUT_DIR%%EXEC%