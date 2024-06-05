from OrbMaths import *
from TleParser import *
from TleFiles import *
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

filename: str = "TLEs/active.tle"
lookingFor: int = 25544

def PrintTle(TLE_OBJECT: TLE = None) -> None:
	OrbPeriod = OrbitalPeriod(TLE_OBJECT.MeanMotion)
	SMA = SemiMajorAxis(OrbPeriod)
	n = AngularSpeed(SMA)

	Ap = Apogee(TLE_OBJECT.Eccentricity, SMA)
	Pe = Perigee(TLE_OBJECT.Eccentricity, SMA)

	Epoch_E = NewtonRaphson(radians(TLE_OBJECT.MeanAnomaly), TLE_OBJECT.Eccentricity, KeplerEquation, KeplerPrime, radians(TLE_OBJECT.MeanAnomaly), 1e-10, 0xffffffffffffffff)

	Epoch_R = OrbAlt(TLE_OBJECT.Eccentricity, SMA, Epoch_E)
	Epoch_Alt = Epoch_R - EARTH_RADIUS

	Speed_Ap = OrbSpeed(Ap, SMA)
	Speed_Pe = OrbSpeed(Pe, SMA)
	Speed_Epoch = OrbSpeed(Epoch_R, SMA)

	utc = datetime.now(UTC)
	current_year = utc.year
	current_day = ((utc - datetime(current_year,1,1, tzinfo=UTC)).days + 1) + utc.hour/24 + utc.minute/1440 + utc.second / 86400 + utc.microsecond/86400000000

	epoch_year = TLE_OBJECT.EPOCH_YR

	if (TLE_OBJECT.EPOCH_YR < 57):
		epoch_year += 2000
	else:
		epoch_year += 1900

	DeltaTime = ((current_year - epoch_year) * 365.25 + (current_day - TLE_OBJECT.EPOCH)) * 86400

	Current_MA = radians(TLE_OBJECT.MeanAnomaly) + n * DeltaTime

	Current_E = NewtonRaphson(Current_MA, TLE_OBJECT.Eccentricity, KeplerEquation, KeplerPrime, Current_MA, 1e-10, 0xffffffffffffffff)

	Current_MA = degrees(Current_MA)
	Current_MA %= 360.0

	Current_R = OrbAlt(TLE_OBJECT.Eccentricity, SMA, Current_E)
	Current_Alt = Current_R - EARTH_RADIUS
	Current_Spd = OrbSpeed(Current_R, SMA)

	output = f"""Object name : {TLE_OBJECT.name}
---------------------------------- TLE ----------------------------------
NORAD ID : {TLE_OBJECT.NORAD_ID:0>5}{TLE_OBJECT.Classification}
COSPAR : {TLE_OBJECT.COSPAR_YR} {TLE_OBJECT.COSPAR_LN:0>3} {TLE_OBJECT.COSPAR_OBJ_ID}
EPOCH : YEAR={epoch_year} DAY={TLE_OBJECT.EPOCH:.8f}
TLE AGE : {secstohms(DeltaTime)}
(MEAN MOTION)' = {TLE_OBJECT.FIRST_DERIV_MEAN_MOTION:.8f}
(MEAN MOTION)'' = {TLE_OBJECT.SECOND_DERIV_MEAN_MOTION:.5f}
B* = {TLE_OBJECT.B_STAR:.5e}

INCLINATION : {TLE_OBJECT.Inclination:.4f} degs
LONGITUDE OF ASC. NODE : {TLE_OBJECT.AscNodeLong:.4f} degs
ECCENTRICITY : {TLE_OBJECT.Eccentricity:.7f}
ARG. OF PERIGEE : {TLE_OBJECT.PeriArg:.4f} degs
MEAN ANOMALY : {TLE_OBJECT.MeanAnomaly:.4f} degs
MEAN MOTION : {TLE_OBJECT.MeanMotion:.8f} rev/(sid. day)
-------------------------------- RESULTS --------------------------------
Orbital Period : {OrbPeriod:.4f} secs ({secstohms(OrbPeriod)})
Semi Major Axis : {SMA:_} m
Apogee : {int(Ap-EARTH_RADIUS):_} m | Perigee : {int(Pe-EARTH_RADIUS):_} m | Epoch : {int(Epoch_Alt):_} m
Speed @ Ap : {Speed_Ap:.4f} m/s | Pe : {Speed_Pe:.4f} m/s | Ep : {Speed_Epoch:.4f} m/s 
------------------------------- CURRENTLY -------------------------------
DATE (UTC) : {utc.day:0>2}/{utc.month:0>2}/{utc.year:0>4} {utc.hour:0>2}:{utc.minute:0>2}:{utc.second:0>2}.{utc.microsecond:0>6}
MEAN ANOMALY : {Current_MA:.4f} ({degrees(n * DeltaTime)}) degs
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
			sleep(1/5)

	return 0

main()