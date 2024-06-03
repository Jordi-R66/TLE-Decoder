from math import *

def strtoscinotd(original: str) -> float:
	total_shift: int = 0

	intermediate_string = ["" for i in range(9)]

	for i in range(8):
		c = original[i]

		if ((c == "+") or (c == "-")) and i > 0:
			total_shift = 1
			intermediate_string[i] = "e"

		intermediate_string[i+total_shift] = c
	
	intermediate_string = "".join(intermediate_string)
	final_output: float = float(intermediate_string)/10

	return final_output

def secstohms(secs: float) -> str:
	d_days = secs/86400.0;
	days = int(d_days);

	d_hours = (d_days - days) * 24.0;
	hours = int(d_hours);

	d_minutes = (d_hours - hours) * 60.0;
	minutes = int(d_minutes);
	
	d_seconds = (d_minutes - minutes) * 60.0;
	seconds = int(d_seconds);

	centiseconds = int((d_seconds - seconds) * 100);

	output = f"{days:0>2}d {hours:0>2}h {minutes:0>2}m {seconds:0>2}.{centiseconds:0>2}s"

	return output