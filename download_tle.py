from requests import get
from time import time, time_ns

SETS: list[str] = sorted([
	"active",
	"stations",
	"last-30-days",
	"visual",
	"analyst",
	"cosmos-1408-debris",
	"fengyun-1c-debris",
	"iridium-33-debris",
	"cosmos-2251-debris",
	"weather",
	"noaa",
	"goes",
	"resource",
	"sarsat",
	"dmc",
	"tdrss",
	"argos",
	"planet",
	"spire",
	"geo",
	"gpz",
	"gpz-plus",
	"intelsat",
	"ses",
	"starlink",
	"iridium",
	"iridium-NEXT",
	"oneweb",
	"orbcomm",
	"globalstar",
	"swarm",
	"amateur",
	"x-comm",
	"other-comm",
	"satnogs",
	"gorizont",
	"raduga",
	"molniya",
	"gnss",
	"gps-ops",
	"glo-ops",
	"galileo",
	"beidou",
	"sbas",
	"nnss",
	"musson",
	"science",
	"geodetic",
	"engineering",
	"education",
	"military",
	"radar",
	"cubesat",
	"other"
])

BASE_URL: str = "https://celestrak.org/NORAD/elements/gp.php?GROUP=SET_NAME&FORMAT=tle"

def download_set(set_name: str):
	START: float = time_ns()
	REQ_URL: str = BASE_URL.replace("SET_NAME", set_name)
	req = get(REQ_URL)

	if ("not found" in req.text):
		print(f"Downloading {set_name} as SPECIAL...")
		REQ_URL = REQ_URL.replace("GROUP", "SPECIAL")
		req = get(REQ_URL)

	response_bytes = req.content

	filename: str = f"TLEs/{set_name}.tle"

	with open(filename, "wb") as fp:
		for c in response_bytes:
			if (c != ord("\r")):
				fp.write(chr(c).encode("ascii"))

	return time_ns() - START

def download_all():
	total_time: int = 0
	exec_time: int = 0
	for set_name in SETS:
		print(f"Downloading {set_name}")
		exec_time = download_set(set_name)
		print(f"{set_name} succesfully downloaded in {exec_time/1_000_000_000:.3}s !")
		total_time += exec_time
	
	print(f"All files have been downloaded in {total_time/1_000_000_000:.3}s")

def merge_all():
	final_file = open("TLEs/merged.tle", "wb")

	merged_ids = []

	total_time: int = 0
	exec_time: int = 0

	for set_name in SETS:
		print(f"Merging {set_name}.tle")
		START: int = time_ns()
		filename = f"TLEs/{set_name}.tle"
		current_file = open(filename, "rb")
		lines = current_file.readlines()
		current_file.close()

		n_block = len(lines)//3
		
		for i in range(n_block):
			block = lines[i*3: i*3+3]
			NORAD_ID = int(block[2][2:7])
			
			if NORAD_ID not in merged_ids:
				merged_ids.append(NORAD_ID)
				final_file.writelines(block)
		END: int = time_ns()

		exec_time = END - START
		total_time += exec_time

		print(f"{set_name}.tle merged in {exec_time/1_000_000_000:.3}s")
	
	print(f"All files has been merged in {total_time/1_000_000_000:.3}s")

	final_file.close()

if __name__ == "__main__":
	download_all()
	merge_all()