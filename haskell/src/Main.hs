module Main where

main :: IO ()
main = putStrLn "Hello!"

readExample :: IO ()
readExample = do
  thing <- getLine
  putStrLn ("What the fuck is \"" ++ thing ++ "\"?")
  return ()
