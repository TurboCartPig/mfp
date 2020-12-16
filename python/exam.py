#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@author: Dennis Kristiansen, NTNU
"""

lst0 = list(range(10000))

# i)
# lst1 = [3 * v + 1 for v in lst0]
lst1 = list(map(lambda x: 3 * x + 1, lst0))
print(lst1)

# ii)
# lst2 = [v for v in lst1 if v % 101 == 0]
lst2 = list(filter(lambda x: x % 101 == 0, lst1))
print(lst2)

# iii)
# lst3 = [(lambda x: (lambda y: x + y))(x) for x in lst2]
lst3 = list(map(lambda x: (lambda y: x + y), lst2))
print(lst3)

# iv)
# lst4 = [(lst2[i], lst3[i]) for i in range(len(lst2))]
lst4 = list(zip(lst2, lst3))
print(lst4)

# v)
# lst5 = [x[1](x[0]) for x in lst4]
lst5 = list(map(lambda x: x[1](x[0]), lst4))
print(lst5)

# vi)
# lst1 = list(map(lambda x: x * x, range(100)))
lst1 = [x * x for x in range(100)]
print(lst1)

# vii)
# lst2 = list(map(lambda x: sum(map(lambda y: 2 * y, range(0,x))), range(100)))
lst2 = [2 * y for x in range(100) for y in range(0, x)]
print(lst2)

# viii)
# lst3 = list(map(lambda x: x[0] - x[1], zip(lst1, lst2)))
lst3 = [lst1[i] - lst2[i] for i in range(len(lst1))]
print(lst3)

# ix)
lst0 = list(range(100))
# for i in lst0:
#     for j in lst0:
#         if i*j > 5000:
#             print(i,"*",j,"=",i*j)

[print(i, "*", j, "=", i * j) for i in lst0 for j in lst0 if i * j > 5000]

# x)
lst1 = []
for i in range(100):
    lst1.append([])
    for j in range(100):
        lst1[-1].append(j)

k = 100
for j in lst1:
    for l in range(k):
        j.pop()
    k -= 1

for l in lst1:
    print(l)


# NOTE: Assumes that the printout is all that matters
# NOTE: lst1 doesn't actually contain anything here
lst1 = list(map(print, (map(lambda x: list(range(x)), range(100)))))


print("\nProbability:\n")

from itertools import combinations, product, permutations

dice = [1, 2, 3, 4, 5, 6]
S = list(product(dice, repeat=5))


def unique(roll):
    return roll[0] != roll[1] != roll[2] != roll[3] != roll[4]


E = [roll for roll in S if unique(roll)]

# Printout: Probability is 3750 / 7776
print("Probability is", len(E), "/", len(S))

people = ["a", "b", "c", "d", "e"]
S = list(permutations(people, len(people)))


def next_to(perm):
    index_1 = perm.index("a")
    index_10 = perm.index("b")
    return index_1 + 1 == index_10 or index_1 - 1 == index_10


E = [p for p in S if next_to(p)]

# Printout: Probability is 48 / 120
print("Probability is", len(E), "/", len(S))

deck = list(range(52))
S = list(combinations(deck, 4))


def value(card):
    return card // 4


def same(hand):
    return value(hand[0]) == value(hand[1]) == value(hand[2]) == value(hand[3])


E = [hand for hand in S if same(hand)]

# Printout: Probability is 13 / 270725
print("Probability is", len(E), "/", len(S))

deck = list(range(52))
S = list(combinations(deck, 4))


def two_pairs(hand):
    hand = sorted(map(value, hand))
    return hand[0] == hand[1] and hand[2] == hand[3]


E = [hand for hand in S if two_pairs(hand) and not same(hand)]

# Printout: Probability is 2808 / 270725
print("Probability is", len(E), "/", len(S))
