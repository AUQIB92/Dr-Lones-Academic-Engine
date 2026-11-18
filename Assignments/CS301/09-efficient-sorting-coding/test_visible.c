/*
 * test_visible.c -- the visible tests, one per worked example in the Week 9
 * coding problem statement. Run locally to sanity-check your sorting.c before
 * submitting; the autograder runs a LARGER hidden suite on top of these.
 *
 * Build + run (from the assignment directory):
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c sorting.c
 *   ./test_visible
 */

#include <stdio.h>
#include <string.h>
#include "sorting.h"

static int failures = 0;

static void check_array(const char *name, const int *got,
                        const int *want, int n)
{
    int ok = 1;
    for (int i = 0; i < n; i++)
        if (got[i] != want[i]) ok = 0;
    if (ok) {
        printf("  PASS  %s\n", name);
    } else {
        printf("  FAIL  %s (got [", name);
        for (int i = 0; i < n; i++)
            printf("%d%s", got[i], i + 1 < n ? "," : "");
        printf("], want [");
        for (int i = 0; i < n; i++)
            printf("%d%s", want[i], i + 1 < n ? "," : "");
        printf("])\n");
        failures++;
    }
}

static void check_ll(const char *name, long long got, long long want)
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
    printf("Example 1: mergeRuns\n");
    {
        int A[6] = { 1, 4, 7, 2, 5, 8 };
        int tmp[6];
        int want[6] = { 1, 2, 4, 5, 7, 8 };
        mergeRuns(A, 0, 2, 5, tmp);
        check_array("mergeRuns([1,4,7 | 2,5,8])", A, want, 6);
    }

    printf("Example 2: mergeSort\n");
    {
        int A[7] = { 5, 1, 4, 2, 8, 0, 3 };
        int want[7] = { 0, 1, 2, 3, 4, 5, 8 };
        mergeSort(A, 7);
        check_array("mergeSort([5,1,4,2,8,0,3])", A, want, 7);
    }
    {
        int A[1] = { 42 };
        mergeSort(A, 1);
        check_array("mergeSort(single)", A, (int[]){42}, 1);
    }

    printf("Example 3: quickSort (adversarial inputs)\n");
    {
        int A[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        int want[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        quickSort(A, 8);
        check_array("quickSort(sorted)", A, want, 8);
    }
    {
        int A[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };
        int want[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        quickSort(A, 8);
        check_array("quickSort(reverse)", A, want, 8);
    }
    {
        int A[5] = { 4, 4, 4, 4, 4 };
        int want[5] = { 4, 4, 4, 4, 4 };
        quickSort(A, 5);
        check_array("quickSort(all equal)", A, want, 5);
    }
    {
        int A[6] = { 3, 1, 2, 1, 3, 2 };
        int want[6] = { 1, 1, 2, 2, 3, 3 };
        quickSort(A, 6);
        check_array("quickSort(duplicates)", A, want, 6);
    }

    printf("Example 4: shellSort\n");
    {
        int A[9] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        int want[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        shellSort(A, 9);
        check_array("shellSort(reverse)", A, want, 9);
    }
    {
        int A[6] = { 3, 1, 2, 1, 3, 2 };
        int want[6] = { 1, 1, 2, 2, 3, 3 };
        shellSort(A, 6);
        check_array("shellSort(duplicates)", A, want, 6);
    }

    printf("Example 5: countInversions\n");
    {
        int A[4] = { 2, 4, 1, 3 };
        check_ll("countInversions([2,4,1,3]) -> 3",
                 countInversions(A, 4), 3);
    }
    {
        int A[3] = { 1, 2, 3 };
        check_ll("countInversions([1,2,3]) -> 0",
                 countInversions(A, 3), 0);
    }
    {
        int A[3] = { 3, 2, 1 };
        check_ll("countInversions([3,2,1]) -> 3",
                 countInversions(A, 3), 3);
    }
    {
        int A[3] = { 2, 2, 2 };
        check_ll("countInversions([2,2,2]) -> 0",
                 countInversions(A, 3), 0);
    }
    {
        int A[5] = { 5, 4, 3, 2, 1 };
        check_ll("countInversions([5,4,3,2,1]) -> 10",
                 countInversions(A, 5), 10);
    }

    if (failures == 0) {
        printf("\nAll visible tests passed.\n");
        return 0;
    }
    printf("\n%d visible test(s) FAILED.\n", failures);
    return 1;
}
