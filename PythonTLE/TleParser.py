from __future__ import annotations
from CONVERSIONS import *

NAME_LENGTH: int = 25-1

NORAD_ID_LENGTH: int = 6-1
YR_LENGTH: int = 3-1
LAUNCH_NB_LENGTH: int = 4-1
LAUNCH_PART_LENGTH: int = 4-1
EPOCH_LENGTH: int = 13-1
DERIV_1_LENGTH: int = 11-1
DERIV_2_LENGTH: int = 9-1
BSTAR_LENGTH: int = 9-1

INCLINATION_LENGTH: int = 9-1
ASC_NODE_LENGTH: int = 9-1
ECC_LENGTH: int = 8-1
ARG_PE_LENGTH: int = 9-1
MEAN_ANO_LENGTH: int = 9-1
MEAN_MOTION_LENGTH: int = 12-1

REVS_LENGTH: int = 6-1

COSPAR_LENGTH: int = (YR_LENGTH + LAUNCH_NB_LENGTH + LAUNCH_PART_LENGTH)

class TLE:
	def __init__(self, name: str = None, NORAD_ID: int = None, Classification: str = None, COSPAR_YR: int = None, COSPAR_LN: int = None, COSPAR_OBJ_ID: str = None, EPOCH_YR: int = None, EPOCH: float = None, FIRST_DERIV_MEAN_MOTION: float = None, SECOND_DERIV_MEAN_MOTION: float = None, B_STAR: float = None, Inclination: float = None, AscNodeLong: float = None, Eccentricity: float = None, PeriArg: float = None, MeanAnomaly: float = None, MeanMotion: float = None, Revolutions: int = None):
		self.name = name
		self.NORAD_ID = NORAD_ID
		self.Classification = Classification
		self.COSPAR_YR = COSPAR_YR
		self.COSPAR_LN = COSPAR_LN
		self.COSPAR_OBJ_ID = COSPAR_OBJ_ID
		self.EPOCH_YR = EPOCH_YR
		self.EPOCH = EPOCH
		self.FIRST_DERIV_MEAN_MOTION = FIRST_DERIV_MEAN_MOTION
		self.SECOND_DERIV_MEAN_MOTION = SECOND_DERIV_MEAN_MOTION
		self.B_STAR = B_STAR
		self.Inclination = Inclination
		self.AscNodeLong = AscNodeLong
		self.Eccentricity = Eccentricity
		self.PeriArg = PeriArg
		self.MeanAnomaly = MeanAnomaly
		self.MeanMotion = MeanMotion
		self.Revolutions = Revolutions

def parse_block(lines: list[str]) -> TLE:
	NAME_LINE: str = lines[0]
	FIRST_LINE: list[str] = list(lines[1])
	SECOND_LINE: list[str] = list(lines[2])

	# 0-th line

	OBJ_NAME: str = ""

	# 1st line

	NORAD_CAT: list[str] = ["" for i in range(NORAD_ID_LENGTH)]
	CLASSIFICATION: str = "0"
	COSPAR_YR: list[str] = ["" for i in range(YR_LENGTH)]
	LAUNCH_NB: list[str] = ["" for i in range(LAUNCH_NB_LENGTH)]
	LAUNCH_PART: list[str] = ["" for i in range(LAUNCH_PART_LENGTH)]
	EPOCH_YR: list[str] = ["" for i in range(YR_LENGTH)]
	EPOCH_DAY: list[str] = ["" for i in range(EPOCH_LENGTH)]
	DERIV_1: list[str] = ["" for i in range(DERIV_1_LENGTH)]
	DERIV_2: list[str] = ["" for i in range(DERIV_2_LENGTH)]
	BSTAR: list[str] = ["" for i in range(BSTAR_LENGTH)]

	# 2nd line

	INCLI: list[str] = ["" for i in range(INCLINATION_LENGTH)]
	AN: list[str] = ["" for i in range(ASC_NODE_LENGTH)]
	ECC: list[str] = ["." for i in range(ECC_LENGTH+1)]
	ARG_PE: list[str] = ["" for i in range(ARG_PE_LENGTH)]
	MEAN_ANO: list[str] = ["" for i in range(MEAN_ANO_LENGTH)]
	MEAN_MOTION: list[str] = ["" for i in range(MEAN_MOTION_LENGTH)]
	REVOLUTIONS: list[str] = ["" for i in range(REVS_LENGTH)]

	# --------------------------- 1-ST LINE PARSING ---------------------------

	OBJ_NAME = NAME_LINE

	for i in range(NORAD_ID_LENGTH):
		c = FIRST_LINE[i+2]

		if i < 5:
			NORAD_CAT[i] = c
		else:
			CLASSIFICATION = c

	for i in range(COSPAR_LENGTH):
		c = FIRST_LINE[i+9]

		if i < 2:
			COSPAR_YR[i] = c
		elif i < 5:
			LAUNCH_NB[i-2] = c
		else:
			LAUNCH_PART[i-5] = c

	for i in range(EPOCH_LENGTH + YR_LENGTH):
		c = FIRST_LINE[i+18]

		if i < (YR_LENGTH):
			EPOCH_YR[i] = c
		else:
			EPOCH_DAY[i-2] = c

	for i in range(DERIV_1_LENGTH):
		c = FIRST_LINE[i+33]

		DERIV_1[i] = c

	for i in range(DERIV_2_LENGTH):
		c = FIRST_LINE[i+44]

		DERIV_2[i] = c

	for i in range(BSTAR_LENGTH):
		c = FIRST_LINE[i+53]

		BSTAR[i] = c

	# --------------------------- 2-ND LINE PARSING ---------------------------

	for i in range(NORAD_ID_LENGTH):
		if (NORAD_CAT[i] != SECOND_LINE[i+2]):
			print("%s\n%s\n", FIRST_LINE, SECOND_LINE)
			Exception("The NORAD Catalogue Number doesn't match up between line 1 and 2\n")

	for i in range(INCLINATION_LENGTH):
		c = SECOND_LINE[i+8]

		INCLI[i] = c

	for i in range(ASC_NODE_LENGTH):
		c = SECOND_LINE[i+17]

		AN[i] = c

	for i in range(ECC_LENGTH):
		c = SECOND_LINE[i+26]

		ECC[i+1] = c

	for i in range(ARG_PE_LENGTH):
		c = SECOND_LINE[i+34]

		ARG_PE[i] = c

	for i in range(MEAN_ANO_LENGTH):
		c = SECOND_LINE[i+43]

		MEAN_ANO[i] = c

	for i in range(MEAN_MOTION_LENGTH):
		c = SECOND_LINE[i+52]

		MEAN_MOTION[i] = c

	for i in range(REVS_LENGTH):
		c = SECOND_LINE[i+63]

		REVOLUTIONS[i] = c

	output: TLE = TLE()

	output.name = OBJ_NAME

	output.NORAD_ID = int("".join(NORAD_CAT))
	output.Classification = CLASSIFICATION

	output.COSPAR_YR = int("".join(COSPAR_YR))
	output.COSPAR_LN = int(int("".join(LAUNCH_NB)))
	output.COSPAR_OBJ_ID = "".join(LAUNCH_PART)

	output.EPOCH_YR = int("".join(EPOCH_YR))
	output.EPOCH = float("".join(EPOCH_DAY))

	output.FIRST_DERIV_MEAN_MOTION = float("".join(DERIV_1)) * 2
	output.SECOND_DERIV_MEAN_MOTION = strtoscinotd("".join(DERIV_2)) * 6
	output.B_STAR = strtoscinotd("".join(BSTAR))

	output.Inclination = float("".join(INCLI))
	output.AscNodeLong = float("".join(AN))
	output.Eccentricity = float("".join(ECC))
	output.PeriArg = float("".join(ARG_PE))
	output.MeanAnomaly = float("".join(MEAN_ANO))
	output.MeanMotion = float("".join(MEAN_MOTION))

	output.Revolutions = int("".join(REVOLUTIONS))

	return output

def parse_lines(NAME_LINE: str, FIRST_LINE: str, SECOND_LINE: str) -> TLE:
	lines = [NAME_LINE, FIRST_LINE, SECOND_LINE]

	return parse_block(lines)
