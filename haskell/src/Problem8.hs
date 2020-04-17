module Problem8 where

-- | My List
data MyList = Cons Int MyList | Empty
  deriving (Show)

-- Problem 8.1
-- The type of f = Cons 20 is: f :: MyList -> MyList
-- Is is a partialy applied function, ie. it's missing the last parameter in order to return a list

-- Problem 8.2

-- | Lenght of a "MyList"
mylen :: MyList -> Int
mylen Empty = 0
mylen (Cons _ xs) = 1 + mylen xs

-- Problem 8.3

-- | Sum of elements in a "MyList"
mysum :: MyList -> Int
mysum Empty = 0
mysum (Cons x xs) = x + mysum xs
