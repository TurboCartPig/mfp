module Problem10 where

-- I'll be using haskell's own implementations for the given functions.
-- Simply to learn them, and how to read haskell documentation.

-- Problem 10.1
-- Just some experiments with functors

ex1 :: [String]
ex1 = map show range
  where
    range = [1 .. 10] :: [Int]

ex2 :: [Int]
ex2 =
  let range = [1 .. 10] :: [Int]
   in filter (\x -> x `mod` 2 == 0) range

-- Problem 10.2

-- Add 10 to a value
myfun1 :: Int -> Int
myfun1 = (10 +)

-- Alias for the addition operator with prefix syntax
myfun2 :: Int -> Int -> Int
myfun2 = (+)

-- Is the parameter smaller then 10?
myfun3 :: Int -> Bool
myfun3 = (10 >)

-- Double the parameter
myfun4 :: Int -> Int
myfun4 = (2 *)

-- Alias for the equality operator with prefix syntax
myfun5 :: Int -> Int -> Bool
myfun5 = (==)

-- Problem 10.3

lst1 :: [Int]
lst1 = filter (100 <) [1 .. 200]

lst2 :: [Double]
lst2 = map fromIntegral ([1 .. 200] :: [Int])

-- Problem 10.4
isZero :: Int -> Bool
isZero = (==) 0
-- isZero -1 doesn't do what we expect because,
-- instead of calling isZero with a parameter of -1,
-- we are here trying to call (-) (The subtraction operator)
-- with the parameters of (Int -> Bool) and Int, which doesn't make sense.
--
-- In order to call isZero with the parameter -1, we have to write:
-- isZero (-1)
