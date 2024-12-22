def cheksum_algorithm(line: str, modulo: int) -> int:
	checksum: int = 0
	line_length: int = len(line)

	if line_length != 69: return -1

	line = line.encode("ascii")

	for c in line:
		if (48 <= c <= 57):
			c -= 48
		elif (c == b"-"):
			c = 1
		else:
			c = 0
		
		checksum += c

	checksum %= modulo

	return checksum

def AntecedentDroite(a: float, b: float, y: float) -> float:
	return (y - b) / a

def NewtonRaphson(target, func_param, func, func_prime, x_start: float, tolerance: float, max_iter: int) -> float:
	xf = 0
	x_guess = x_start

	low_limit = target - tolerance
	high_limit = target + tolerance

	n: int = 0

	while (n < max_iter) and not ((low_limit <= func(x_guess, func_param)) and (func(x_guess, func_param) <= high_limit)):
		a, b = func_prime(x_guess, func_param), -func_prime(x_guess, func_param)*x_guess + func(x_guess, func_param)
		x_guess = AntecedentDroite(a, b, target)
		n += 1

	xf = x_guess

	return xf