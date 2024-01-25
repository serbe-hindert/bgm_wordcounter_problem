# SOLUTION WAY

1. Read file into array
2. define line number as 0
3. go through every letter of array
   1. if current character is new line THEN
      1. increase line number by one
      2. skip to next iteration
   2. find out the current words word length
   3. if the word matches the search word THEN
      1. add the word to the tuples
   4. OTHERWISE
      1. increase iteration by word length