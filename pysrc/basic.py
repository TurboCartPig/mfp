#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar  4 08:22:20 2020

@author: dennis
"""

print("Hello")

a = 1.0
print(type(a))
b = 20
c = a + b
print(c)

#
# Containers (list, tuples)
#

# List
mlist = [1,2,3,4,5,6]
print(mlist)
print(mlist[-1])
print(len(mlist))

# Slicing
# [start:end]
print(mlist[1:-1])
# [start:end:step]
print(mlist[0:-2:2])

# Task
# Print 30, 50, 70
mylist = [10,20,30,40,50,60,70,80,90]
print(mylist[2:-2:2])

#
# Loops
# 

# for loop
for x in mylist:
    print(x * x)
    print(2 * x)

for i in range(50):
    print(i * i)
    
mylist = list(range(5, 100, 5))
print(mylist)

for i in range(30):
    if i > 10 and i < 20:
        print(i, "is bigger than 10 and less than 20")
    elif i % 2 == 1 or i < 10:
        print(i, "is odd or less then 10")
    else:
        print(i, "sucks")

# Task
# Print primes less than 100
# This is wrong
for i in range(2, 100):
    prime = True
    for j in [2, 3, 5, 7]:
        if i % j != 0:
            prime = False
    print(i, "is ", prime)
    
# Task
# Print odd squares less then 100
for i in range(1, 10, 2):
    print(i * i)
    
    
# While loop
while i < 100:
    print(i)
    i += 1
    
#
# List comprehension
# [expr for var in container if predicate]
mylist = [x*x for x in range(20)]
print(mylist)
mylist2 = [x for x in mylist if x < 100]
print(mylist2)
mylist3 = [x*y for x in mylist for y in mylist2]
print(mylist3)

# 
# Lists of lists
#
mylist = [[1,2,3],4,"test",[5,6,7]]
print(len(mylist))
print(mylist[0][1])

# Task
# [[1], [1,2], ..., [1,2,..10]]
mylist = [list(range(1,x+1)) for x in range(1,11)]
print(mylist)

#
# Tuples
# Note: Tuples are immutable, but otherwise similar to lists
# Does support inner mutation

mytuple = (1,2,3,4)
print(mytuple)

mylist = list(mytuple)
print(mylist)

#
# Gererators - Not a tuple
#

mylist = (x for x in range(10))
for x in mylist:
    print(x)
print(list(mylist))

#
# Modules
#
# import random
# random.foo()
#
# import random as bob
# bob.foo()
#
# from random import foo, bar
# foo()

import random as rnd
print(rnd.random())
print(rnd.randint(1,50))

# Task
# Create list with 100 random numbers [0,1]
mylist = [rnd.random() for x in range(100)]
print(mylist)

mylist = [[1,2,3],[],[4,5]]
# Create a list which contains len of mylist
mylens = [len(x) for x in mylist]
print(mylens)

# Card example
# 
deck = list(range(52))
print(deck)

def suit(card):
    return card % 4

def value(card):
    return card // 4

print(suit(2), value(2))

# Lambdas - annonymous functions
suit = lambda card: card % 4

import itertools as it

mylist = [1,2,3,4,5]
comb   = it.combinations(mylist, 2) # 5C2 = 10
print(list(comb))

# All possible 5 card combinations from a deck of cards
sample = list(range(1,53))
comb   = it.combinations(sample, 5)
print(len(list(comb)))

# Least one ace - ace = 1, 14, 27, 40
def numAce(hand):
    return len([x for x in hand if x == 1 or x == 14 or x == 27 or x == 40])

mylist = [x for x in sample if numAce(x) > 0]
print(mylist)



