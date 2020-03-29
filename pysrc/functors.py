from functools import reduce
from itertools import combinations, product, permutations

# More python assignment

# a)
# listcomp solution
# l = range(1000000)
# l = [x * x for x in l]
# l = [x for x in l if x % 4 == 3]
# print(list(l))

# Functor solution
l = range(1000000)
l = map(lambda x: x ** 2, l)
l = filter(lambda x: x % 4 == 3, l)
print(list(l))

# b)
# Provided sample implementation
#
# Opening doors 1..1024
#
# open every door
# close every second door starting with 2
# open/close every third door starting with 3
# ..
# ..
# open/close door 1024
#
# doors = [False for i in range(1024)]
# for i in range(1024):
#     for j in range(i, 1024, i + 1):
#         doors[j] = not doors[j]
# doors = [i + 1 for i in range(1024) if doors[i]]
# print(doors)

doors = [False for i in range(1024)]
for i in range(1024):
    doors[i::i+1] = [not state for state in doors[i::i+1]]
doors = [i + 1 for i in range(1024) if doors[i]]
print(doors)


# Set all doors to close
doors = list(map(lambda _: False, range(1024)))

# TODO: Can I do this differently, ie. avoid using subscripting?
for i in range(1024):
    doors[i::i+1] = map(lambda door: not door, doors[i::i+1])

# List door numbers for open doors
doors = map(lambda tu: tu[0] + 1, filter(lambda tu: tu[1], enumerate(doors)))
print(list(doors))

# c)
# Probability of at least 4 sixes from 7 fair dice
dice = [1, 2, 3, 4, 5, 6]
S = list(product(dice, repeat=7))
E = list(filter(lambda roll: len(list(filter(lambda x: x == 6, roll))) >= 4, S))  # Note

# Printout: Probability is 4936 / 279936
print("Probability is", len(E), "/", len(S))


# Probability of sum = 35 from 7 fair dice
dice = [1, 2, 3, 4, 5, 6]
S = list(product(dice, repeat=7))
E = list(filter(lambda roll: sum(roll) == 35, S))  # Note

# Printout: Probability is 1667 / 279936
print("Probability is", len(E), "/", len(S))


# Probability of drawing a full house from normal playing deck
deck = range(52)
S = list(combinations(deck, 5))


def value(card):
    return card // 4


def full_house(hand):
    hand = sorted(map(lambda card: value(card), hand))  # Note
    # 2 first and 2 last are equal and the middle card is equal to ether the first 2 or the last 2
    return hand[0] == hand[1] and hand[3] == hand[4] and (hand[2] == hand[0] or hand[2] == hand[3])


E = list(filter(lambda hand: full_house(hand), S))  # Note

# Printout: Probability is 3744 / 2598960
print("Probability is", len(E), "/", len(S))


# Probability of drawing 4 jacks in a hand of 5 cards
cards = list(range(52))
S = list(combinations(cards, 5))
E = list(filter(lambda jacks: jacks == 4,
                map(lambda hand: len(list(filter(lambda card: value(card) == 11, hand))), S)))  # Note

# Printout: Probability is 48 / 2598960
print("Probability is", len(E), "/", len(S))


# Probability of person 1 being next to person 10 in a queue of 10 people
people = list(range(1, 11))
S = list(permutations(people, len(people)))


def next_to(perm):
    index_1 = perm.index(1)
    index_10 = perm.index(10)
    return index_1 + 1 == index_10 or index_1 - 1 == index_10


E = list(filter(lambda p: next_to(p), S))  # Note

# Printout: Probability is 725760 / 3628800
print("Probability is", len(E), "/", len(S))


# d)
def envelop(l):
    return map(lambda x: [x], l)


def last(l):
    return map(lambda x: x[-1], l)


def length(l):
    return map(lambda x: len(x), l)


# Test
print(list(envelop([1, 2, 3])))  # Prints [[1], [2], [3]]
print(list(last([[1], [2], [3, 4, 5]])))  # Prints [1, 2, 5]
print(list(length([[1], [2, 2], [3, 3, 3]])))  # Prints [1, 2, 3]

# e)
# maps the print function to individual elements of the collection
# elements in l are strings
# elements in s are chars
l = list(map(print, ["Hope", "You", "Are", "Well!"]))
s = list(map(print, "Hope You Are Well!"))


# f)
def product(l):
    return reduce(lambda a, b: a * b, l)


# Test
print(product([1, 2, 3]))
print(product([1, 2, 3, 4, 5]))


# g) - 1
# Assuming I am allowed to use enumerate()
def take(n, l):
    return map(lambda tu: tu[1], filter(lambda tu: tu[0] < n, enumerate(l)))


# If not
def take2(n, l):
    return map(lambda tu: tu[0], zip(l, range(n)))


# Test
print(list(take(2, [1, 2, 3])))
print(list(take2(2, [1, 2, 3])))


# g) - 2
# Assumes I can use range in solution
# Effectively reimplements enumerate
def drop(n, l):
    return map(lambda tu: tu[1], filter(lambda tu: tu[0] >= n, zip(range(len(l)), l)))


# Test
print(list(range(10)))
print(list(drop(3, range(10))))
