module Problem7 where

-- Could we have used | Vec3 Vec2 Float
data Vector = Vec2 Float Float | Vec3 Float Float Float
  deriving (Show)

-- | Lenght of a "Vector"
veclen :: Vector -> Float
veclen (Vec2 x y) = sqrt (x * x + y * y)
veclen (Vec3 x y z) = sqrt (x * x + y * y + z * z)

-- Problem 7.1

-- | "Vector" addition
addvec :: Vector -> Vector -> Vector
addvec (Vec2 x1 y1) (Vec2 x2 y2) = Vec2 (x1 + x2) (y1 + y2)
addvec (Vec2 x1 y1) (Vec3 x2 y2 z2) = Vec3 (x1 + x2) (y1 + y2) z2
addvec (Vec3 x1 y1 z1) (Vec2 x2 y2) = Vec3 (x1 + x2) (y1 + y2) z1
addvec (Vec3 x1 y1 z1) (Vec3 x2 y2 z2) = Vec3 (x1 + x2) (y1 + y2) (z1 + z2)

-- | Scaler multiplication with "Vector"
scalmul :: Float -> Vector -> Vector
scalmul a (Vec2 x y) = Vec2 (a * x) (a * y)
scalmul a (Vec3 x y z) = Vec3 (a * x) (a * y) (a * z)

-- | Normalization of a "Vector"
normalize :: Vector -> Vector
normalize v = scalmul (1 / veclen v) v

-- Problem 7.2
--
-- f in f = Vec3 10 20 has the type:
-- f :: Float -> Vector
-- It is a partialy applied function that will return a Vector given a Float
--
-- f in f = veclen . (Vec2 20) has the type:
-- f :: Float -> Float
-- It is a function applied to a partialy applied function. Once Vec2 has gotten y, veclen will be run with the vector as a paramerter

-- Problem 7.3

-- | Custom bool sum type
data MyBool
  = -- | True
    T
  | -- | False
    F
  deriving (Show)

-- The constructors of this type are T and F? Or are there no constructors?
-- Don't know

-- | Exclusive or
myxor :: MyBool -> MyBool -> MyBool
myxor T F = T
myxor F T = T
myxor _ _ = F

-- | And
myand :: MyBool -> MyBool -> MyBool
myand T T = T
myand F F = T
myand _ _ = F

-- If we were to derive "Eq":
-- myand a b
--   | a == b = T
--   | otherwise = F

-- | Not
mynot :: MyBool -> MyBool
mynot x = case x of
  F -> T
  T -> F

-- Problem 7.4

-- | Dot product between "Vector"s
-- NOTE: The order of the patterns
dot :: Vector -> Vector -> Float
dot (Vec2 x1 y1) (Vec2 x2 y2) = x1 * x2 + y1 * y2
dot (Vec3 x1 y1 z1) (Vec3 x2 y2 z2) = x1 * x2 + y1 * y2 + z1 * z2
dot v (Vec3 x y _) = dot v $ Vec2 x y
dot (Vec3 x y _) v = dot v $ Vec2 x y
