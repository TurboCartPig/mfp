module Problem4 where

-- Problem 4.1
clamp1 :: Float -> Float
clamp1 x =
  if x < 0 
    then 0
  else if x > 1
    then 1
  else x

clamp2 :: Float -> Float
clamp2 x
  | x < 0 = 0
  | x > 1 = 1
  | otherwise = x

-- Problem 4.2
greaterThan10 :: Int -> Bool
greaterThan10 x
  | x > 10 = True
  | otherwise = False

lessThan10 :: Int -> Bool
lessThan10 x
  | x < 10 = True
  | otherwise = False

equalTo10 :: Int -> Bool
equalTo10 x
  | not (greaterThan10 x || lessThan10 x) = True
  | otherwise = False
