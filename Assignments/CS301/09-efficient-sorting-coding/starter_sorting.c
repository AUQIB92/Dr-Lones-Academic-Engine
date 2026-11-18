/*
 * starter_sorting.c -- Coding Assignment 9 (Efficient Sorting).
 *
 * Copy this file to sorting.c and implement the five functions declared in
 * sorting.h. Compile with:
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c sorting.c
 *   ./test_visible
 *
 * The stubs below compile but do nothing useful; each one is replaced by the
 * algorithm from the Week 9 deck. Read the header comments in sorting.h for
 * the exact contract, including the complexity limits the hidden tests check.
 */

#include <stdlib.h>
#include "sorting.h"

/* Merge two adjacent sorted runs A[lo..mid] and A[mid+1..hi] via tmp. */
void mergeRuns(int *A, int lo, int mid, int hi, int *tmp)
{
    /* TODO: two-finger merge; on ties take the left run first. */
    (void)A;
    (void)lo;
    (void)mid;
    (void)hi;
    (void)tmp;
}

/* Recursive merge sort over A[0..n-1]. */
void mergeSort(int *A, int n)
{
    /* TODO: allocate one buffer, recurse, merge, free the buffer. */
    (void)A;
    (void)n;
}

/* Quick sort over A[0..n-1] with a median-of-three or randomized pivot. */
void quickSort(int *A, int n)
{
    /* TODO: partition around a good pivot and recurse on both sides. */
    (void)A;
    (void)n;
}

/* Shell sort over A[0..n-1] with a gap sequence ending at 1. */
void shellSort(int *A, int n)
{
    /* TODO: diminishing-increment insertion sort. */
    (void)A;
    (void)n;
}

/* Number of inversions in A[0..n-1], in O(n log n), without modifying A. */
long long countInversions(const int *A, int n)
{
    /* TODO: merge-sort-style counting on a copy of A. */
    (void)A;
    (void)n;
    return 0;
}
