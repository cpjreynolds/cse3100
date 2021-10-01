#include "subsetsum.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * This function has 3 inputs and will produce on stdout the list of subsetsum
 * in lexicographic increasing order
 * n : the number of integers in the input set
 * s : the target sum to reach
 * t : an array of size n with the integers present in the input set.
 * output: none. Only side-effects on the standard output
 */
void subSetSum(int n, int s, int* t)
{
    if (n == 0) {
        return;
    }
    subSetSum(n - 1, s, t);
    // subSetSum(n - 1, s - t[n - 1], t);

    int buf[n];
    int* bp = buf; // stack of ints.
    int sum = 0;
    for (int i = n - 1; i >= 0 && sum < s; i--) {
        if (sum + t[i] <= s) {
            *bp++ = t[i];
            sum += t[i];
        }
    }
    if (sum == s) {
        printf("sss:[");
        while (bp != buf) {
            printf("%d ", *--bp);
        }
        printf("]\n");
    }
}

void ss_inner(int n, int s, int* t)
{
    if (n == 0) {
        return;
    }

    int buf[n];
    int* bp = buf; // stack of ints.
    int sum = 0;
    for (int i = n - 1; i >= 0 && sum < s; i--) {
        if (sum + t[i] <= s) {
            *bp++ = t[i];
            sum += t[i];
        }
    }
    if (sum == s) {
        while (bp != buf) {
            printf("%d ", *--bp);
        }
        printf("]\n");
    }
}
