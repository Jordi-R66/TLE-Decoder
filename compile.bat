cls
set COMPILER=gcc

set SOURCES=libs/TleFiles.c libs/OrbMaths.c libs/TleParser.c libs/CONVERSIONS.c libs/Algos.c libs/TermFuncs.c libs/Kepler.c libs/Ellipses.c ext_libs/myOwnCLib/matrices/matrix.c ext_libs/myOwnCLib/vectors/vectors.c main.c

set OUTPUT_DIR=output/

mkdir %OUTPUT_DIR%

set EXEC=TLE.exe

set ARGS=-g -O0 -std=c23
REM set ARGS=-O3 -std=c17 -m64 -march=x86-64 -flto

%COMPILER% %ARGS% %SOURCES% -o %OUTPUT_DIR%%EXEC%