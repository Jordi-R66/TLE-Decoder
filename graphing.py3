import matplotlib.pyplot as plt

X = []

Ys = [[], [], []]

fp = open('log.csv', 'r')

content = fp.read().split('\n')[1::]
fp.close()

for line in content:
	if line == '':
		continue

	cols = line.split(',')

	X.append(int(cols[0]))
	Ys[0].append(float(cols[1]))
	Ys[1].append(float(cols[2]))
	Ys[2].append(float(cols[3]))

plt.plot(X, Ys[0], label='AltCoords')
plt.plot(X, Ys[1], label='AltEccAno')
plt.plot(X, Ys[2], label='AltTrueAno')

plt.legend()
plt.xlabel('Time (s)')

plt.ylabel('Altitude (m)')

plt.savefig('graph.png')