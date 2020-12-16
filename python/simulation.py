#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 10 14:31:03 2020

@author: Dennis Kristiansen, NTNU
"""

from random import randint, random, shuffle, uniform, choice

# Simulation of #
num_sim = 100000
hits = 0

for i in range(num_sim):
    su = sum([randint(1, 6) for x in range(10)])
    if su == 30:
        hits += 1

# Probability is 0.04909
print("Probability is", hits / num_sim)


# Simulation of person 1 being infront of person 10 in a random queue
num_sim = 100000
hits = 0
people = list(range(1, 11))

for i in range(num_sim):
    shuffle(people)
    if people.index(1) > people.index(10):
        hits += 1

# Probability is 0.50111
print("Probability is", hits / num_sim)


# Simulation of calculating area of intersection between 2 circles
# with radius = 2 and coords = (0, 0) and (2, 0)
num_sim = 100000
hits = 0
circles = [(0.0, 0.0), (2.0, 0.0)]
radius = 2

for i in range(num_sim):
    # Random coord in encapsulating box
    coord = (uniform(-2.0, 4.0), uniform(-2.0, 2.0))
    intersections = len(
        [
            c
            for c in circles
            if ((coord[0] - c[0]) ** 2 + (coord[1] - c[1]) ** 2) <= radius ** 2
        ]
    )
    # Hit both, in intersection
    if intersections == 2:
        hits += 1

area = 24 * hits / num_sim

# The area is 4.88976
print("The area is", area)


# Simulation of probablility of rollinig all sixes with 20 fair dice
num_sim = 100000
hits = 0

for i in range(num_sim):
    su = sum([randint(1, 6) for x in range(20)])
    if su == 6 * 20:
        hits += 1

# Probability is ~0.0
print("Probability is", hits / num_sim)


# Simulation that shows that changing doors
# is the best strategy for the Monty Hall problem
num_sim = 100000
hits = 0

# Stick with your original choice
for i in range(num_sim):
    doors = [True, False, False]
    shuffle(doors)
    guest = randint(0, 2)
    guest = doors[guest]
    # Host actions do not matter
    if guest:
        hits += 1

# Probability when sticking to your choice = 0.33338
print("Probability when sticking to your choice", hits / num_sim)


# Switch door
hits = 0

for i in range(num_sim):
    doors = [True, False, False]
    shuffle(doors)
    guest = randint(0, 2)
    del doors[guest]
    host = doors.index(False)  # Host removes a goat
    del doors[host]
    guest = doors[0]  # Switches to other door
    if guest:
        hits += 1

# Probability when swithing door = 0.66657
print("Probability when switching door", hits / num_sim)
