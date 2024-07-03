from TleParser import TLE, parse_block

def GetTLENumber(filename: str) -> int:
	line_count: int = 0

	with open(filename, "r", encoding="ascii") as f:
		content = list(f.read())
		line_count += content.count("\n")
		f.close()
	
	return int(line_count // 3)

def GetAllTle(filename: str) -> list[TLE]:
	TLE_NUMBER: int = GetTLENumber(filename)
	BLOCK_SIZE: int = 3

	Output: list[TLE] = []

	with open(filename, "r", encoding="ascii") as tle_file:
		raw_content: list[str] = tle_file.read().split("\n")

		for i in range(TLE_NUMBER):
			j_start = 0 + BLOCK_SIZE * i
			j_end = BLOCK_SIZE + BLOCK_SIZE * i

			extract = raw_content[j_start:j_end]

			if "UNKNOWN" not in extract[0]:
				Output.append(parse_block(extract))

		tle_file.close()

	return Output
