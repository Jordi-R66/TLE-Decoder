from OrbMaths import *
from TleParser import *
from TleFiles import *
from Ellipses import *
from Algos import NewtonRaphson

from math import degrees, radians
from datetime import datetime, UTC

from sys import platform
from os import system
from time import sleep

if platform == "win32":
	CLEAR_CMD = "cls"
else:
	CLEAR_CMD = "clear"

filename: str = "TLEs/stations.tle"
lookingFor: int = 25544

EccentricAnomalyTolerance: float = radians(0.0001)
DEFAULT_ITER: int = 100000

def PrintTle(Object: TLE = None) -> None:
	OrbPeriod: float = OrbitalPeriod(Object.MeanMotion)
	SMA: int = SemiMajorAxis(OrbPeriod)
	n: float = AngularSpeed(SMA)

	Ap: int = Apoapsis(Object.Eccentricity, SMA)
	Pe: int = Periapsis(Object.Eccentricity, SMA)

	Epoch_E: float = NewtonRaphson(radians(Object.MeanAnomaly), Object.Eccentricity, KeplerEquation, KeplerPrime, radians(Object.MeanAnomaly), EccentricAnomalyTolerance, DEFAULT_ITER)
	Epoch_TA: float = TrueAnomaly(Object.Eccentricity, Epoch_E)

	Epoch_R: float = OrbAltTA(Object.Eccentricity, SMA, Epoch_TA)
	Epoch_Alt: float = Epoch_R - EARTH_RADIUS

	Speed_Ap: float = OrbSpeed(Ap, SMA)
	Speed_Pe: float = OrbSpeed(Pe, SMA)
	Speed_Epoch: float = OrbSpeed(Epoch_R, SMA)

	utc: datetime = datetime.now(UTC)
	current_year = utc.year
	current_day: float = ((utc - datetime(current_year, 1, 1, tzinfo=UTC)).days + 1) + utc.hour/24 + utc.minute/1440 + utc.second / 86400 + utc.microsecond/86400000000

	epoch_year: int = Object.EPOCH_YR

	if (Object.EPOCH_YR < 57):
		epoch_year += 2000
	else:
		epoch_year += 1900

	DeltaTime: float = ((current_year - epoch_year) * 365.25 + (current_day - Object.EPOCH)) * 86400

	Current_MA: float = radians(Object.MeanAnomaly) + n * DeltaTime
	Current_E: float = NewtonRaphson(Current_MA, Object.Eccentricity, KeplerEquation, KeplerPrime, Current_MA, EccentricAnomalyTolerance, DEFAULT_ITER)
	Current_TA: float = TrueAnomaly(Object.Eccentricity, Current_E)

	Current_R: float = OrbAltTA(Object.Eccentricity, SMA, Current_TA)
	Current_Alt: float = Current_R - EARTH_RADIUS
	Current_Spd: float = OrbSpeed(Current_R, SMA)

	Current_MA = degrees(Current_MA)
	Current_MA %= 360.0

	Current_TA = degrees(Current_TA)
	Current_TA %= 360.0

	# output = f"Epoch MA : {Object.MeanAnomaly}\nCurrent MA : {Current_MA}\n\n"

	output: str = f"""Object name : {Object.name}
---------------------------------- TLE ----------------------------------
NORAD ID : {Object.NORAD_ID:0>5}{Object.Classification}
COSPAR : {Object.COSPAR_YR} {Object.COSPAR_LN:0>3} {Object.COSPAR_ID}
EPOCH : YEAR={epoch_year} DAY={Object.EPOCH:.8f}
TLE AGE : {secstohms(DeltaTime)}
(MEAN MOTION)' = {Object.FIRST_DERIV_MEAN_MOTION:.8f}
(MEAN MOTION)'' = {Object.SECOND_DERIV_MEAN_MOTION:.5f}
B* = {Object.B_STAR:.5e}

INCLINATION : {Object.Inclination:.4f} degs
LONGITUDE OF ASC. NODE : {Object.AscNodeLong:.4f} degs
ECCENTRICITY : {Object.Eccentricity:.7f}
ARG. OF PERIAPSIS : {Object.PeriArg:.4f} degs
MEAN ANOMALY : {Object.MeanAnomaly:.4f} degs
MEAN MOTION : {Object.MeanMotion:.8f} rev/(sid. day)
-------------------------------- RESULTS --------------------------------
Orbital Period : {OrbPeriod:.4f} secs ({secstohms(OrbPeriod)})
Semi Major Axis : {SMA:_} m
Apoapsis : {int(Ap-EARTH_RADIUS):_} m | Periapsis : {int(Pe-EARTH_RADIUS):_} m | Epoch : {int(Epoch_Alt):_} m
Speed @ Ap : {Speed_Ap:.4f} m/s | Pe : {Speed_Pe:.4f} m/s | Ep : {Speed_Epoch:.4f} m/s 
------------------------------- CURRENTLY -------------------------------
DATE (UTC) : {utc.day:0>2}/{utc.month:0>2}/{utc.year:0>4} {utc.hour:0>2}:{utc.minute:0>2}:{utc.second:0>2}
MEAN ANOMALY : {Current_MA:.4f} degs
ECC. ANOMALY : {Current_E:.4f} rads
TRUE ANOMALY : {Current_TA:.4f} degs
ALTITUDE : {int(Current_Alt):_} m
SPEED : {Current_Spd:.4f} m/s""".replace("_", " ")

	print(output)

def main() -> int:
	system(CLEAR_CMD)

	AllObjs: list[TLE] = GetAllTle(filename)
	block_quant: int = len(AllObjs)

	print(f"{block_quant} entries loaded\nLooping to find {lookingFor:0>2}\n\n")

	CurrentEntry: TLE = TLE()
	found: bool = False

	for i in range(block_quant):
		CurrentEntry = AllObjs[i]
		if (CurrentEntry.NORAD_ID == lookingFor):
			found = True
			break;

	if found:
		while True:
			system(CLEAR_CMD)
			PrintTle(CurrentEntry)
			sleep(1/2)

	return 0

main()