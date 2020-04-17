module Maybe where

ex1 :: Int -> Int -> Maybe Int
ex1 x y = do
  a <- Just x
  b <- Just y
  return $ a + b
