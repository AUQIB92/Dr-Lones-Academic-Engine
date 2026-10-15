/*
 * starter_search.c -- starter stub for Week 8's institute practical
 * (binary search and its variants).
 *
 * Copy this file to search.c and fill in every function body. Do not modify
 * search.h. Each function below is a TODO: replace the placeholder body with
 * the deck's algorithm (Slides/CS301/08-searching-elementary-sorting.tex,
 * Section 1 for the three search routines and Section 2 for isqrt), keeping
 * the contract stated in search.h.
 *
 * Build + sanity-check (from this directory):
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c search.c
 *   ./test_visible
 */

#include <stddef.h>
#include "search.h"

int binarySearch(const int *A, int n, int key)
{
    (void)A;
    (void)n;
    (void)key;
    return -1; /* TODO: lo = 0, hi = n - 1; halve while lo <= hi; mid = lo + (hi-lo)/2 */
}

int lowerBound(const int *A, int n, int key)
{
    (void)A;
    (void)n;
    (void)key;
    return 0; /* TODO: half-open [lo, hi); if A[mid] < key shift lo=mid+1 else keep hi=mid */
}

int upperBound(const int *A, int n, int key)
{
    (void)A;
    (void)n;
    (void)key;
    return 0; /* TODO: same as lowerBound but keep hi=mid only when A[mid] > key */
}

int countOccurrences(const int *A, int n, int key)
{
    (void)A;
    (void)n;
    (void)key;
    return 0; /* TODO: upperBound(A, n, key) - lowerBound(A, n, key); no scan */
}

int isqrtFloor(int n)
{
    (void)n;
    return 0; /* TODO: binary search the predicate (long long)k*k <= n over 0..n */
}
