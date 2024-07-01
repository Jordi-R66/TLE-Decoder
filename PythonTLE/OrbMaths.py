from math import *

EARTH_DAY_LENGTH: float = 86400

G: float = 6.67428e-11
EARTH_MASS: float = 5.9722e24
POLAR_RADIUS: int = 6356752
EQUA_RADIUS: int = 6378137

EARTH_RADIUS: float = (2*EQUA_RADIUS + POLAR_RADIUS)//3

EARTH_MU: int = int(G*EARTH_MASS)

C: int = 299_792_458

def OrbitalPeriod(MeanMotion: float) -> float:
	Period: float = 0.0

	Period = EARTH_DAY_LENGTH/MeanMotion

	return Period

def SemiMajorAxis(Period: float) -> int:
	SMA: int = 0.0

	SMA = int(cbrt((Period/(2*pi))**2 * EARTH_MU))

	return SMA



def Apoapsis(Eccentricity: float, SMA: int) -> int:
	Apoapsis: int = 0

	Apoapsis = int(SMA * (1 + Eccentricity))

	return Apoapsis

def Periapsis(Eccentricity: float, SMA: int) -> int:
	Periapsis: int = 0

	Periapsis = int(SMA * (1 - Eccentricity))

	return Periapsis

def OrbAlt(Eccentricity: float, SMA: int, E: float) -> int:
	OrbAlt: int = 0

	OrbAlt = int(SMA * (1 - Eccentricity * cos(E)))

	return OrbAlt

def OrbAltTA(Eccentricity: float, SMA: int, TrueAnomaly: float) -> float:
	return SMA * (1-Eccentricity**2) / (1+Eccentricity * cos(TrueAnomaly))



def KeplerEquation(E: float, e: float) -> float:
	return E - e*sin(E)

def KeplerPrime(E: float, e: float) -> float:
	return 1 - e*cos(E)

def AngularSpeed(SMA: int) -> float:
	return sqrt(EARTH_MU/SMA**3)

def TrueAnomaly(Eccentricity: float, EccentricAnomaly: float) -> float:
	return 2 * atan(sqrt((1+Eccentricity)/(1-Eccentricity)) * tan(EccentricAnomaly/2))

# def Beta(Eccentricity: float) -> float:
# 	return Eccentricity/(1+sqrt(1-Eccentricity**2))

# def TrueAnomaly(Eccentricity: float, EccentricAnomaly: float) -> float:
# 	A: float = 1+Eccentricity
# 	B: float = 1-Eccentricity
# 	C: float = tan(EccentricAnomaly/2)

# 	print(A * C/B)

# 	return sqrt(A * C/B)

# def TrueAnomaly(Eccentricity: float, EccentricAnomaly: float) -> float:
# 	return EccentricAnomaly + 2 * atan((Beta(Eccentricity) * sin(EccentricAnomaly))/(1 - Beta(Eccentricity) * cos(EccentricAnomaly)))

# def SIN_TA(Eccentricity: float, EccentricAnomaly: float) -> float:
# 	A = sqrt(1-Eccentricity**2) * sin(EccentricAnomaly)
# 	B = KeplerPrime(EccentricAnomaly, Eccentricity)

# 	return A/B

# def COS_TA(Eccentricity: float, EccentricAnomaly: float) -> float:
# 	A = cos(EccentricAnomaly) - Eccentricity
# 	B = KeplerPrime(EccentricAnomaly, Eccentricity)

# 	return A/B

# def TrueAnomaly(Eccentricity: float, EccentricAnomaly: float) -> float:
# 	return atan(SIN_TA(Eccentricity, EccentricAnomaly)/COS_TA(Eccentricity, EccentricAnomaly))



def OrbSpeed(altitude: int, SMA: int) -> float:
	speed: float = 0.0

	speed = sqrt(EARTH_MU * (2/altitude - 1/SMA))

	return speed

def Lorentz(speed: float) -> float:
	lorentz: float = 0.0

	lorentz = 1 / sqrt(1 - speed**2/C**2)

	return lorentz
