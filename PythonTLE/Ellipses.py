from math import *

def b(a: float, e: float) -> float:
	if not (0 <= e < 1):
		raise Exception("Eccentricity must be 0 <= e < 1")

	return a * sqrt(1 - e**2)

def c(a: float, e: float) -> float:
	if not (0 <= e < 1):
		raise Exception("Eccentricity must be 0 <= e < 1")

	return a * e

def h(a: float, e: float) -> float:
	if not (0 < e < 1):
		raise Exception("Eccentricity must be 0 < e < 1")

	return (a * (1 - e**2)) / e

def f(a: float, e: float) -> float:
	if not (0 < e < 1):
		raise Exception("Eccentricity must be 0 < e < 1")

	return a / e

def p(a: float, e: float) -> float:
	if not (0 <= e < 1):
		raise Exception("Eccentricity must be 0 <= e < 1")

	return a * (1 - e**2)

# ---------------------------------------------------------------------

def r(TrueAnomaly: float, p: float, e: float) -> float:
	if not (0 <= e < 1):
		raise Exception("Eccentricity must be 0 <= e < 1")

	return p / (1 + e * cos(TrueAnomaly))

def InitialX_2D(TrueAnomaly: float, Dist: float) -> float:
	if (Dist <= 0):
		raise Exception("Distance must be strictly higher than 0")

	return Dist * cos(TrueAnomaly)

def InitialY_2D(TrueAnomaly: float, Dist: float) -> float:
	if (Dist <= 0):
		raise Exception("Distance must be strictly higher than 0")

	return Dist * sin(TrueAnomaly)

def ActualX_2D(x: float, y: float, PeAng: float) -> float:
	return x * cos(PeAng) - y * sin(PeAng)

def ActualY_2D(x: float, y: float, PeAng: float) -> float:
	return x * sin(PeAng) + y * cos(PeAng)