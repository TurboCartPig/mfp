#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 10 14:30:44 2020

@author: dennis
"""

from itertools import combinations, product, permutations

# Probability of at least 4 sixes from 7 fair dice
dice = [1, 2, 3, 4, 5, 6]
S = list(product(dice, repeat=7))


def num_sixes(roll):
    return len([x for x in roll if x == 6])


E = [roll for roll in S if num_sixes(roll) >= 4]

# Printout: Probability is 4936 / 279936
print("Probability is", len(E), "/", len(S))

# Probability of sum = 35 from 7 fair dice
dice = [1, 2, 3, 4, 5, 6]
S = list(product(dice, repeat=7))

E = [roll for roll in S if sum(roll) == 35]

# Printout: Probability is 1667 / 279936
print("Probability is", len(E), "/", len(S))

# Probability of drawing a full house from normal playing deck
deck = list(range(52))
S = list(combinations(deck, 5))


def value(card):
    return card // 4


def full_house(hand):
    hand = [value(x) for x in hand]
    hand.sort()
    # 2 first and 2 last are equal and the middle card is equal to ether the first 2 or the last 2
    return hand[0] == hand[1] and hand[3] == hand[4] and (hand[2] == hand[0] or hand[2] == hand[3])


E = [hand for hand in S if full_house(hand)]

# Printout: Probability is 3744 / 2598960
print("Probability is", len(E), "/", len(S))

# Probability of drawing 4 jacks in a hand of 5 cards
cards = list(range(52))
S = list(combinations(cards, 5))


def num_jacks(hand):
    return len([card for card in hand if value(card) == 11])


E = [x for x in S if num_jacks(x) == 4]

# Printout: Probability is 48 / 2598960
print("Probability is", len(E), "/", len(S))

# Probability of person 1 being next to person 10 in a queue of 10 people
people = list(range(1, 11))
S = list(permutations(people, len(people)))


def next_to(perm):
    index_1 = perm.index(1)
    index_10 = perm.index(10)
    return index_1 + 1 == index_10 or index_1 - 1 == index_10


E = [p for p in S if next_to(p)]

# Printout: Probability is 725760 / 3628800
print("Probability is", len(E), "/", len(S))