/*
 * test_visible.c -- the visible tests, one per worked example in the Week 8
 * coding problem statement. Run locally to sanity-check your search.c before
 * submitting; the autograder runs a LARGER hidden suite on top of these.
 *
 * Build + run (from the assignment directory):
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c search.c
 *   ./test_visible
 */

#include <stdio.h>
#include "search.h"

static int failures = 0;

static void check(const char *name, long long got, long long want)
{
    if (got == want) {
        printf("  PASS  %s\n", name);
    } else {
        printf("  FAIL  %s (got %lld, want %lld)\n", name, got, want);
        failures++;
    }
}

int main(void)
{
    /* Sorted identifier table used by Examples 1 and 2. */
    static const int A[8] = { 4, 9, 15, 15, 22, 31, 47, 58 };
    const int n = 8;

    /* Example 1: ordinary binary search -- hit at 0/middle/last, and misses. */
    printf("Example 1: binarySearch on [4,9,15,15,22,31,47,58]\n");
    check("binarySearch(22) -> 4", binarySearch(A, n, 22), 4);
    check("binarySearch(4) -> 0", binarySearch(A, n, 4), 0);
    check("binarySearch(58) -> 7", binarySearch(A, n, 58), 7);
    check("binarySearch(16) absent -> -1", binarySearch(A, n, 16), -1);
    check("binarySearch(100) absent -> -1", binarySearch(A, n, 100), -1);
    check("binarySearch on empty -> -1", binarySearch(A, 0, 4), -1);

    /* Example 2: the two bounds and the occurrence count. */
    printf("Example 2: lowerBound / upperBound / countOccurrences\n");
    check("lowerBound(15) -> 2", lowerBound(A, n, 15), 2);
    check("upperBound(15) -> 4", upperBound(A, n, 15), 4);
    check("countOccurrences(15) -> 2", countOccurrences(A, n, 15), 2);
    check("lowerBound(16) -> 4", lowerBound(A, n, 16), 4);
    check("upperBound(16) -> 4", upperBound(A, n, 16), 4);
    check("countOccurrences(16) -> 0", countOccurrences(A, n, 16), 0);
    check("lowerBound(1) below all -> 0", lowerBound(A, n, 1), 0);
    check("upperBound(1) below all -> 0", upperBound(A, n, 1), 0);
    check("countOccurrences(1) -> 0", countOccurrences(A, n, 1), 0);
    check("lowerBound(99) above all -> 8", lowerBound(A, n, 99), 8);
    check("upperBound(99) above all -> 8", upperBound(A, n, 99), 8);
    check("countOccurrences(99) -> 0", countOccurrences(A, n, 99), 0);
    check("lowerBound(4) -> 0", lowerBound(A, n, 4), 0);
    check("upperBound(4) -> 1", upperBound(A, n, 4), 1);
    check("countOccurrences(4) -> 1", countOccurrences(A, n, 4), 1);
    check("lowerBound(58) -> 7", lowerBound(A, n, 58), 7);
    check("upperBound(58) -> 8", upperBound(A, n, 58), 8);

    /* Example 3: integer square root by binary search on a monotone predicate. */
    printf("Example 3: isqrtFloor (no floating point)\n");
    check("isqrtFloor(0) -> 0", isqrtFloor(0), 0);
    check("isqrtFloor(1) -> 1", isqrtFloor(1), 1);
    check("isqrtFloor(2) -> 1", isqrtFloor(2), 1);
    check("isqrtFloor(3) -> 1", isqrtFloor(3), 1);
    check("isqrtFloor(4) -> 2", isqrtFloor(4), 2);
    check("isqrtFloor(15) -> 3", isqrtFloor(15), 3);
    check("isqrtFloor(16) -> 4", isqrtFloor(16), 4);
    check("isqrtFloor(17) -> 4", isqrtFloor(17), 4);
    check("isqrtFloor(26) -> 5", isqrtFloor(26), 5);
    check("isqrtFloor(48) -> 6", isqrtFloor(48), 6);
    check("isqrtFloor(63) -> 7", isqrtFloor(63), 7);

    if (failures == 0) {
        printf("\nAll visible tests passed.\n");
        return 0;
    }
    printf("\n%d visible test(s) FAILED.\n", failures);
    return 1;
}
