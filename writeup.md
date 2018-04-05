# CS 304 Programming Assignment 2

Author: Xida Ren

## HOWTO

Run `make test` to generate `output_test.txt`. Beware that
the testing process is done in the backgorund. The process
may be resource-intensive, and `output_test.txt` will not
be complete until performance testing concludes. Data in 
this document are taken from `output_test.txt`.

## Correctness

All three algorthms corectly selects the k-smallest element of the array
{7,  2, 4,  6, 9, 11, 2,  6, 10, 6, 15, 6, 14, 2, 7, 5,  13, 9, 12, 15}

|         k|    algorithm 1|    algorithm 2|    algorithm 3|
|:--------:|---------------|---------------|---------------|  
|         0|              2|              2|              2|
|         1|              2|              2|              2|
|         2|              2|              2|              2|
|         3|              4|              4|              4|
|         4|              5|              5|              5|
|         5|              6|              6|              6|
|         6|              6|              6|              6|
|         7|              6|              6|              6|
|         8|              6|              6|              6|
|         9|              7|              7|              7|
|        10|              7|              7|              7|
|        11|              9|              9|              9|
|        12|              9|              9|              9|
|        13|             10|             10|             10|
|        14|             11|             11|             11|
|        15|             12|             12|             12|
|        16|             13|             13|             13|
|        17|             14|             14|             14|
|        18|             15|             15|             15|
|        19|             15|             15|             15|

## Corner Cases

The three algorithms correctly handles selection from single-element
arrays, and small, sorted arrays. Running the algorithms. over large
random arrays result in values close to the center of the random 
range.

## Performance

This table is generated at the end of testing:

|         n|    algorithm 1|    algorithm 2|    algorithm 3|
|----------|---------------|---------------|---------------|
|    100000|           2.18|          12.38|           9.81|
|    300000|           7.04|          37.40|          34.34|
|   1000000|          31.42|         123.79|         137.40|
|   3000000|          79.32|         371.62|         471.97|
|  10000000|         202.92|        1257.37|        1731.75|
|  30000000|         823.08|        3709.69|        5870.50|
| 100000000|        2755.40|       12267.34|       21693.74|
| 300000000|        7273.97|       36661.93|       69590.02|
|1000000000|       28469.97|      125275.71|      252946.59|

As you can see, algorithm 1 (randomized quickselect) trumps
the rest in performance due to its minimality. Deterministic
quickselect finishes the job in about half the time as
quicksort, as it does not produce a sorted array and saves
a lot of comparisons.