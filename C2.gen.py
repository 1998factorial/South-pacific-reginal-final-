#!/usr/bin/env python3

import random
w = random.randint(2, 10)
h = random.randint(2, 10)
indices = list(range(1, w*h+1))
random.shuffle(indices)

coordinate = {} # map of index to coordinate
grid = []
for r in range(h):
	row = []
	for c in range(w):
		row.append(indices[r*w+c])
		coordinate[indices[r*w+c]] = (r, c)
	grid.append(row)

print(w*h)
for i in range(1, w*h+1):
	neighbours = []
	r = coordinate[i][0]
	c = coordinate[i][1]
	if r != 0: neighbours.append(grid[r-1][c])
	if r != h-1: neighbours.append(grid[r+1][c])
	if c != 0: neighbours.append(grid[r][c-1])
	if c != w-1: neighbours.append(grid[r][c+1])
	print(len(neighbours), end=' ')
	for nxt in neighbours: print(nxt, end=' ')
	print()
