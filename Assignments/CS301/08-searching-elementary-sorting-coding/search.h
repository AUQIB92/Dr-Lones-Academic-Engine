/*
 * search.h -- the contract for Coding Assignment 8 (Searching & the
 * Elementary Sorts), Week 8's institute practical: binary search and its
 * variants.
 *
 * This header is given to you; do NOT change it. Your job is to provide the
 * five functions below in search.c (start from starter_search.c).
 *
 * Setting (the running calculator, Weeks 3-10). The calculator keeps its
 * symbol table as an array of small integer identifier ids, held SORTED in
 * nondecreasing order (Week 8 Section 1: sorting the table is exactly what
 * turns an O(n) lookup into an O(log n) probe). Every function here operates
 * on that sorted table `A` of length `n`.
 *
 * The deck's C for the four algorithms is at
 * Slides/CS301/08-searching-elementary-sorting.tex -- binary search and the
 * lower/upper bound variants (Section 1), and isqrt (Section 2). You write
 * the functions below from that material; the worked examples in the
 * problem statement are fresh data of your own to check against.
 *
 * General precondition for every table function: A[0] <= A[1] <= ... <=
 * A[n-1], and n >= 0. When n == 0 the array is empty and A may be NULL.
 */

#ifndef SEARCH_H
#define SEARCH_H

/*
 * binarySearch: ordinary iterative binary search over the sorted table
 * A[0..n-1].
 *   - return the index of SOME occurrence of `key` (any i with A[i] == key);
 *   - return -1 when `key` does not occur, or when n == 0;
 *   - do NOT modify the table.
 *   - O(log n) worst case, O(1) auxiliary space.
 * The mid computation must avoid the lo + hi overflow: write
 *   mid = lo + (hi - lo) / 2;
 */
int binarySearch(const int *A, int n, int key);

/*
 * lowerBound: the FIRST index i with A[i] >= key (the deck's "first
 * occurrence" boundary), over the sorted table A[0..n-1].
 *   - return the smallest i in [0, n] with A[i] >= key;
 *   - return n when every element is < key (so the boundary is past the end);
 *   - return 0 when n == 0.
 *   - O(log n) worst case.
 * Use a HALF-OPEN range [lo, hi) with hi = n, keep mid as a candidate
 * (hi = mid) when A[mid] >= key, and discard the left half (lo = mid + 1)
 * otherwise.
 */
int lowerBound(const int *A, int n, int key);

/*
 * upperBound: the FIRST index i with A[i] > key --- the same routine as
 * lowerBound with a strict comparison.
 *   - return the smallest i in [0, n] with A[i] > key;
 *   - return n when every element is <= key;
 *   - return 0 when n == 0.
 *   - O(log n) worst case.
 */
int upperBound(const int *A, int n, int key);

/*
 * countOccurrences: how many entries of the sorted table equal `key`. By the
 * deck's identity this is exactly upperBound - lowerBound, so it must run in
 * O(log n) --- do NOT scan the table.
 *   - return the number of i in [0, n) with A[i] == key (0 when none);
 *   - return 0 when n == 0.
 *   - O(log n) worst case; calling your own lowerBound and upperBound is the
 *     intended route.
 */
int countOccurrences(const int *A, int n, int key);

/*
 * isqrtFloor: floor(sqrt(n)), i.e. the largest integer k >= 0 with
 * k*k <= n --- by binary search on the MONOTONE PREDICATE P(k) = (k*k <= n)
 * over the answer space 0..n (the deck's Application 1). The answer is not
 * stored in any array.
 *   - precondition: n >= 0;
 *   - integer arithmetic ONLY: no <math.h>, no floating point, no library
 *     sqrt;
 *   - return the largest feasible k (isqrtFloor(0) == 0, isqrtFloor(1) == 1,
 *     isqrtFloor(2) == 1);
 *   - widen BEFORE squaring so mid*mid cannot overflow: use a signed 64-bit
 *     type, e.g. ((long long)mid * mid <= n). On Windows `long` is only 32
 *     bits -- `(long) mid * mid` overflows for large n (the deck's Week 1
 *     arithmetic caution, made portable).
 *   - O(log n) worst case, O(1) auxiliary space.
 */
int isqrtFloor(int n);

#endif /* SEARCH_H */
