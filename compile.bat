cls
set COMPILER=gcc

set SOURCES=headers/TleFiles.c headers/OrbMaths.c headers/TleParser.c headers/CONVERSIONS.c headers/Algos.c headers/TermFuncs.c headers/Kepler.c headers/Ellipses.c ext_libs/myOwnCLib/matrices/matrix.c ext_libs/myOwnCLib/vectors/vectors.c main.c

set OUTPUT_DIR=output/

mkdir %OUTPUT_DIR%

set EXEC=TLE.exe

set ARGS=-g -O0 -std=c23
REM set ARGS=-O3 -std=c17 -m64 -march=x86-64 -flto

%COMPILER% %ARGS% %SOURCES% -o %OUTPUT_DIR%%EXEC%