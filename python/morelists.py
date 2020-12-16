#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Mar  5 14:14:45 2020

@author: Dennis Kristiansen, NTNU
"""

# Note: b changes - lists passed by referance
a = [1, 2, 3]
b = a
a[1] = 5
print(b)


# Force copy - shallow copy
a = [1, 2, 3]
b = [a.copy(), a.copy()]
a[1] = 200
print(b)


# Problem
a = [1, 2, 3]
b = [a, a]
c = b.copy()
a[1] = 200
print(c)


# Deep copy
from copy import deepcopy

a = [1, 2, 3]
b = [a, a]
c = deepcopy(b)
a[1] = 200
print(c)


# List comprehension - print odd numbers
mylist = [x for x in range(1, 100, 2)]
print(mylist)
mylist = [x for x in range(100) if x % 2 == 1]
print(mylist)
mylist = [2 * n + 1 for n in range(0, 50)]
print(mylist)


# Pick out numbers evenly divisable by 3
mylist2 = [x for x in range(3, 99, 3)]
print(mylist2)
mylist2 = [x for x in range(100) if x % 3 == 0]
print(mylist2)


# Card example
def suit(n):
    return n % 4


def value(n):
    return n // 4


suits = ["clubs", "hearts", "spades", "diamonds"]
print("The suit of card number 34 is", suits[suit(34)])


# Itertools
import itertools as it


# Crating sample spaces
deck = list(range(0, 52))
sample = list(it.combinations(deck, 5))
print(len(sample))


# Assignemts
# At least on ace
def num_aces(hand):
    return len([card for card in hand if value(card) == 0])


event = [
    hand for hand in sample if num_aces(hand) > 0
]  # Every hand with at least one ace

print(
    "The probability is:", len(event), "/", len(sample), "=", len(event) / len(sample)
)

# Dice
dice = list(range(1, 7))
sample = list(it.product(dice, repeat=2))
print(sample)


# Queues
people = [1, 2, 3]
sample = list(it.permutations(people, 4))
print(len(sample))


# Opening doors 1..1024
doors = [False for i in range(1024)]

for i in range(1024):
    doors[i :: i + 1] = [not state for state in doors[i :: i + 1]]
    # for j in range(i, 1024, i + 1):
    #    doors[j] = not doors[j]

# doors = [[not doors[j] for j in range(i, 1024, i + 1)] for i in range(1024)]

doors = [i + 1 for i in range(1024) if doors[i]]
print(doors)

# Import random
import random as rnd
from math import sqrt

# Calculate pi by simulation
num_sim = 100000
hits = 0

for i in range(num_sim):
    x = rnd.random()
    y = rnd.random()

    if sqrt(x * x + y * y) <= 1.0:
        hits += 1

print("The area is:", hits / num_sim)
print("The area of the disk is:", 4 * hits / num_sim)


# Dice simulation
num_sim = 1000000
hits = 0

for i in range(num_sim):
    d1 = rnd.randint(1, 6)
    d2 = rnd.randint(1, 6)

    if d1 == 6 or d2 == 6:
        hits += 1

print("The probability is:", hits / num_sim, 11 / 36)

hits = 0
for i in range(num_sim):
    roll = [rnd.randint(1, 6) for i in range(5)]

    if len([dice for dice in roll if dice == roll[0]]) == 5:
        hits += 1

print("The probability is:", hits / num_sim)


# Card simulation
num_sim = 100000
hits = 0

deck = list(range(0, 52))

for i in range(num_sim):
    rnd.shuffle(deck)
    hand = deck[:5]

    if num_aces(hand) > 2:
        hits += 1

print("The probability is:", hits / num_sim)


# Classes and dictionaries are not covered
# pygame - python game framework
# Reading - map, filter, reduce
