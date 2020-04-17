module Problem5 where

-- Problem 5.1
equalTo10 :: Int -> Bool
equalTo10 = \x -> x == 10

lessThan10 :: Int -> Bool
lessThan10 = \x -> x < 10

greaterThan10 :: Int -> Bool
greaterThan10 = \x -> x > 10

mysum :: Int -> Int -> Int
mysum = \x y -> x + y

myprod :: (Int, Int) -> Int
myprod = \(x, y) -> x * y

-- Problem 5.2
mynot :: (Int -> Bool) -> (Int -> Bool)
mynot f = not . f

myand :: (Int -> Bool) -> (Int -> Bool) -> (Int -> Bool)
myand f g = \x -> f x && g x
