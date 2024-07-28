*Given an array of positive numbers, calculate number of pairs of elements which have a particular gcd value for each possible value of gcd*
Let `dp[i]` denote the number of pairs of elements whose gcd is `i`.
Then `dp[i] = s(s-1)/2 - dp[2i] - dp[3i] - ...` where `s` is the number of multiples of `i` in the array.
Solved in `O(n log n)`, due to harmonic sum.