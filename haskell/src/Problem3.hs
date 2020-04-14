module Problem3 where

-- Problem 3.1
mymul :: Float -> Float -> Float
mymul x y = x * y

doubleF :: Float -> Float
doubleF x = mymul x 2.0

halfF :: Float -> Float
halfF x = mymul x 0.5

-- Problem 3.2
prod :: (Int, Int) -> Int
prod (x, y) = x * y

prod2 :: Int -> Int -> Int
prod2  = curry prod

-- Problem 3.3

-- First returns the first element in a pair
-- fst :: (a, b) -> a

-- Second returns the second element in a pair
-- snd :: (a, b) -> b

-- Problem 3.4

-- Guess:
-- curry fst :: a -> b -> a
-- curry snd :: a -> b -> b

-- Actual:
-- curry fst :: c -> b -> c
-- curry snd :: a -> c -> a

-- curry fst returns the first of two parameters
-- curry snd returns the second of two parameters

-- Example:
-- curry fst "fi" "se" returns "fi"
-- curry snd "fi" "se" returns "se"
