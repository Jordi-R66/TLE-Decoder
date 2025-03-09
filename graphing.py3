from tkinter import Y
import matplotlib.pyplot as plt

X = []

Ys = [[], [], [], []]
Labels = []

fp = open('log_23802.csv', 'r')

content = fp.read().split('\n')
fp.close()

Labels = content[0].split(',')[1::]
content = content[1::]

for line in content:
	if line == '':
		continue

	cols = line.split(',')

	X.append(int(cols[0]))
	Ys[0].append(float(cols[0]))
	Ys[1].append(float(cols[2]))
	Ys[2].append(float(cols[3]))
	Ys[3].append(float(cols[4]))

plt.plot(X, Ys[0], label=Labels[0])
plt.plot(X, Ys[1], label=Labels[1])
plt.plot(X, Ys[2], label=Labels[2])
plt.plot(X, Ys[3], label=Labels[3])

plt.legend()
plt.xlabel('Time (s)')

plt.ylabel('Altitude (m)')

plt.savefig('graph_23802.png')