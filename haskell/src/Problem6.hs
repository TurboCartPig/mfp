module Problem6 where

-- Problem 6.1
sum1 :: Int -> Int
sum1 n
  | n == 1 = 1
  | otherwise = n + sum1 (n - 1)

sum2 :: Int -> Int
sum2 1 = 1
sum2 n = n + sum2 (n - 1)

-- Problem 6.2
mysq :: Int -> Int
mysq x = x * x

-- FIXME: Too low
sum3 :: Int -> Int
sum3 n
  | n == 1 = 1
  | otherwise = mysq n + mysq (n - 1)

sum4 :: Int -> Int
sum4 1 = 1
sum4 n = mysq n + mysq (n - 1)

-- Problem 6.3
mygcd :: (Int, Int) -> Int
mygcd (a, 0) = a
mygcd (a, b) = mygcd (b, mod a b)
