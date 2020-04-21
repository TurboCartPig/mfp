module Problem11 where

import Data.List

-- | Calculate a probability based on an event space and a sample space
-- in the form of lists.
-- NOTE: genericLength is polymorphic over Num,
-- as opposed to length which only returns Ints
probability :: [a] -> [a] -> Double
probability xs ys = genericLength xs / genericLength ys

-- Problem 11.1
-- Sample space of 4 dice
dice :: [Int]
dice = [1 .. 6]

sample1 :: [[Int]]
sample1 = [[i, j, k, l] | i <- dice, j <- dice, k <- dice, l <- dice]

-- Problem 11.2
event1 :: [[Int]]
event1 = [s | s <- sample1, length (filter (== 6) s) >= 2]

probability1 :: Double
probability1 = probability event1 sample1

-- Problem 11.3
-- Sample space of 3 cards in a hand
sample2 :: [[Int]]
sample2 = [[i, j, k] | i <- [0 .. 52], j <- [i + 1 .. 51], k <- [j + 1 .. 51]]

cardValue :: Int -> Int
cardValue card = card `div` 4

event2 :: [[Int]]
event2 = [s | s <- sample2, all (\x -> cardValue x == 11) s]

probability2 :: Double
probability2 = probability event2 sample2

-- Problem 11.4
-- Sample space of 4 people in a queue
sample3 :: [String]
sample3 = permutations ['A', 'B', 'C', 'D']

-- Why do I not have to deal with the fact that elemIndex can fail to lookup?
-- Is that simply part of list comprehension?
-- elemIndex -> Nothing == failed to satisfy predicate?
--
-- I use the index of elements as their ordering in the queue because
-- I wanted the people to be represented using chars,
-- but I could have used ints as elements and had the indecies be the people.
event3 :: [String]
event3 = [s | s <- sample3, elemIndex 'A' s < elemIndex 'B' s]

probability3 :: Double
probability3 = probability event3 sample3
