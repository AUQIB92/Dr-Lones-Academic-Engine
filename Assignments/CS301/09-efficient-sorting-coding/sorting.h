/*
 * sorting.h -- the contract for Coding Assignment 9 (Efficient Sorting),
 * Week 9's institute practicals P9 (quick sort), P10 (shell sort), and
 * P11 (merge sort).
 *
 * This header is given to you; do NOT change it. Your job is to provide the
 * five functions below in sorting.c (start from starter_sorting.c).
 *
 * Setting (the running calculator, Weeks 3-10). The calculator sorts its
 * symbol table once at startup, then searches it many times. The table is an
 * array of integer identifier ids. Every function here sorts or measures such
 * an array in ASCENDING order (nondecreasing), 0-based.
 *
 * The deck's C for the algorithms is at
 * Slides/CS301/09-efficient-sorting.tex -- merge and merge sort (Section 1),
 * Lomuto partition and quick sort (Section 2), heap sort (Section 4), and
 * shell sort (Section 7). You write the functions below from that material;
 * the worked examples in the problem statement are fresh data of your own to
 * check against.
 *
 * General precondition: n >= 0. When n == 0 the array is empty and A may be
 * NULL. Unless a function says otherwise, it may rearrange A in place.
 */

#ifndef SORTING_H
#define SORTING_H

/*
 * mergeRuns: merge two adjacent sorted runs A[lo..mid] and A[mid+1..hi] into
 * one sorted run A[lo..hi].
 *   - precondition: each run is sorted nondecreasing;
 *   - postcondition: A[lo..hi] is sorted nondecreasing;
 *   - tmp points to a caller-provided buffer with at least hi-lo+1 ints;
 *   - on equal keys, take the LEFT run first (stable merge);
 *   - cost: Theta(hi - lo + 1); must not allocate memory.
 */
void mergeRuns(int *A, int lo, int mid, int hi, int *tmp);

/*
 * mergeSort: sort A[0..n-1] ascending with merge sort.
 *   - Theta(n log n) worst case, Theta(n) auxiliary space;
 *   - stable;
 *   - must allocate at most a constant number of buffers (one is enough);
 *   - n == 0 or 1 leaves A unchanged (A may be NULL when n == 0).
 */
void mergeSort(int *A, int n);

/*
 * quickSort: sort A[0..n-1] ascending with quick sort.
 *   - Theta(n log n) average case, O(log n) recursion stack;
 *   - MUST NOT degrade to Theta(n^2) depth on already SORTED, REVERSE-SORTED,
 *     or ALL-EQUAL input: use median-of-three or a randomized pivot (the
 *     lecture's "Choosing a Pivot" section);
 *   - in-place: do not allocate an O(n) buffer;
 *   - not required to be stable;
 *   - n == 0 or 1 leaves A unchanged.
 */
void quickSort(int *A, int n);

/*
 * shellSort: sort A[0..n-1] ascending with shell sort.
 *   - in-place, O(1) auxiliary space (no malloc);
 *   - use a decreasing gap sequence that ends at gap 1 (for example
 *     Knuth's (3^k - 1)/2: ..., 13, 4, 1);
 *   - not required to be stable;
 *   - n == 0 or 1 leaves A unchanged.
 */
void shellSort(int *A, int n);

/*
 * countInversions: return the number of pairs (i, j) with i < j and
 * A[i] > A[j] -- a measure of how far the array is from sorted.
 *   - MUST be O(n log n) worst case: use a merge-sort-style count, not the
 *     O(n^2) double loop;
 *   - MUST NOT modify A (treat it as read-only; copy if you need to sort);
 *   - return a long long: n can reach 200000, and n(n-1)/2 exceeds int;
 *   - return 0 when n == 0.
 */
long long countInversions(const int *A, int n);

#endif /* SORTING_H */
