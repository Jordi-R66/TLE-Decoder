from math import *

EARTH_DAY_LENGTH: float = 86400

G: float = 6.67428e-11#428e-11
EARTH_MASS: float = 5.9722e24
POLAR_RADIUS: int = 6356752
EQUATORIAL_RADIUS: int = 6378137

EARTH_RADIUS: float = 6371000#(2*EQUATORIAL_RADIUS + POLAR_RADIUS)//3

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



def Apogee(Eccentricity: float, SMA: int) -> int:
	Apogee: int = 0

	Apogee = int(SMA * (1 + Eccentricity))

	return Apogee

def Perigee(Eccentricity: float, SMA: int) -> int:
	Perigee: int = 0

	Perigee = int(SMA * (1 - Eccentricity))

	return Perigee

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

def Beta(Eccentricity: float) -> float:
	return Eccentricity/(1+sqrt(1-Eccentricity**2))

# def TrueAnomaly(Eccentricity: float, EccentricAnomaly: float) -> float:
# 	return 2 * atan(sqrt((1+Eccentricity)/(1-Eccentricity) * tan(EccentricAnomaly/2)))

def TrueAnomaly(Eccentricity: float, EccentricAnomaly: float) -> float:
	return EccentricAnomaly + 2 * atan((Beta(Eccentricity) * sin(EccentricAnomaly))/(1 - Beta(Eccentricity) * cos(EccentricAnomaly)))



def OrbSpeed(altitude: int, SMA: int) -> float:
	speed: float = 0.0

	speed = sqrt(EARTH_MU * (2/altitude - 1/SMA))

	return speed

def Lorentz(speed: float) -> float:
	lorentz: float = 0.0

	lorentz = 1 / sqrt(1 - speed**2/C**2)

	return lorentz
