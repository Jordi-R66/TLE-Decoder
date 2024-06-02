from math import *

EARTH_DAY_LENGTH: float = 86400

G: float = 6.67428e-11
EARTH_MASS: float = 5.9722e24
POLAR_RADIUS: int = 6356752
EQUATORIAL_RADIUS: int = 6378137

EARTH_RADIUS: float = (2*EQUATORIAL_RADIUS + POLAR_RADIUS)//3

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

	OrbAlt = int(SMA * (1 + Eccentricity * cos(E)))

	return OrbAlt



def KeplerEquation(E: float, e: float) -> float:
	return E - e*sin(E)

def KeplerPrime(E: float, e: float) -> float:
	return 1 - e*cos(E)

def AngularSpeed(MeanMotion: float) -> float:
	return 1/(OrbitalPeriod(MeanMotion)/(2*pi))

def OrbSpeed(altitude: int, SMA: int) -> float:
	speed: float = 0.0

	speed = sqrt(EARTH_MU * (2/altitude - 1/SMA))

	return speed

def Lorentz(speed: float) -> float:
	lorentz: float = 0.0

	lorentz = 1 / sqrt(1 - speed**2/C**2)

	return lorentz
