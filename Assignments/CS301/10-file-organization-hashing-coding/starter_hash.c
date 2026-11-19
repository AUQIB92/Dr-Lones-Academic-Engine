/*
 * starter_hash.c -- Coding Assignment 10 (Hashing).
 *
 * Copy this file to hash.c and implement the twelve functions declared in
 * hash.h. Compile and run the visible tests with:
 *
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c hash.c
 *   ./test_visible
 *
 * The stubs below compile (that is what the (void) casts are for) but do
 * nothing useful. Read the contract in hash.h before writing code: the
 * return values, the three slot states, and the tombstone rules are all
 * graded, not just "does get find the key".
 *
 * Build order that keeps the work testable:
 *   1. hash_str               -- everything else depends on it;
 *   2. chain_init/put/get/del -- the easy family; get it correct first;
 *   3. chain_probes/chain_free;
 *   4. oa_init/put/get/del    -- reuse the same key-copy discipline;
 *   5. oa_probes/oa_free.
 */

#include <stdlib.h>
#include <string.h>

#include "hash.h"

/* -------------------------------------------------------------------------
 * 1. The hash function
 * ------------------------------------------------------------------------- */

unsigned long hash_str(const char *s, unsigned long m)
{
    /* TODO: h = 0; for each unsigned char c: h = (h * 31 + c) % m; */
    (void)s;
    (void)m;
    return 0;
}

/* -------------------------------------------------------------------------
 * 2-4. Separate chaining
 * ------------------------------------------------------------------------- */

int chain_init(ChainTable *t, size_t m)
{
    /* TODO: reject m == 0, calloc the slot array, set m and n. */
    (void)t;
    (void)m;
    return -1;
}

int chain_put(ChainTable *t, const char *key, int val)
{
    /* TODO: walk the chain for an existing key first; only then allocate. */
    (void)t;
    (void)key;
    (void)val;
    return -1;
}

int chain_get(const ChainTable *t, const char *key, int *out)
{
    /* TODO: walk the chain, compare with strcmp. */
    (void)t;
    (void)key;
    (void)out;
    return 0;
}

int chain_del(ChainTable *t, const char *key)
{
    /* TODO: unlink; remember that the head is a special case. */
    (void)t;
    (void)key;
    return 0;
}

size_t chain_probes(const ChainTable *t, const char *key)
{
    /* TODO: count nodes compared; for an absent key, the whole chain. */
    (void)t;
    (void)key;
    return 0;
}

void chain_free(ChainTable *t)
{
    /* TODO: free each node's key and node, then the slot array. */
    (void)t;
}

/* -------------------------------------------------------------------------
 * 5-7. Open addressing with linear probing and tombstones
 * ------------------------------------------------------------------------- */

int oa_init(OpenTable *t, size_t m)
{
    /* TODO: calloc m slots; every slot starts OA_EMPTY. */
    (void)t;
    (void)m;
    return -1;
}

int oa_put(OpenTable *t, const char *key, int val)
{
    /* TODO: probe; remember the first tombstone; update on a match; insert
     * at the tombstone if one was seen, else at the first EMPTY slot. */
    (void)t;
    (void)key;
    (void)val;
    return -1;
}

int oa_get(const OpenTable *t, const char *key, int *out)
{
    /* TODO: walk over tombstones, stop at EMPTY, stop after m inspections. */
    (void)t;
    (void)key;
    (void)out;
    return 0;
}

int oa_del(OpenTable *t, const char *key)
{
    /* TODO: find, free the key, mark DELETED, decrement n (not used). */
    (void)t;
    (void)key;
    return 0;
}

size_t oa_probes(const OpenTable *t, const char *key)
{
    /* TODO: same walk as oa_get, but return the number of slots inspected. */
    (void)t;
    (void)key;
    return 0;
}

void oa_free(OpenTable *t)
{
    /* TODO: free keys of OCCUPIED slots, then the array. */
    (void)t;
}
