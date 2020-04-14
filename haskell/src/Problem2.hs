module Problem2 where

-- Problem 2.1
doubleF :: Float -> Float
doubleF x = x * 2

halfF :: Float -> Float
halfF x = x / 2

-- Does nothing slowly
noop :: Float -> Float
noop x = halfF $ doubleF x

-- Problem 2.3
mynext :: Int -> Int
mynext x = x + 1

add5_2 :: Int -> Int
add5_2 x = mynext $ mynext $ mynext $ mynext $ mynext x
