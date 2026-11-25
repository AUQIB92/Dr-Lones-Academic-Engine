/*
 * starter_hash.c -- Coding Assignment 10 (File Organization and Hashing).
 *
 * Copy this file to hash.c and implement every function declared in hash.h.
 * Compile with:
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c hash.c
 *   ./test_visible
 *
 * The stubs below compile and run but do nothing useful: create returns NULL,
 * so every table is "not implemented". Read the comments in hash.h for the
 * exact contract -- the probe rule, the resize thresholds, and the tombstone
 * rule are all stated there and are all tested.
 *
 * Suggested order of work:
 *   1. hash_string and hash_int -- get the worked values in the problem
 *      statement to match before building anything on top of them.
 *   2. ChainTable: create, insert by pushing at the head, search, delete,
 *      then add the resize.
 *   3. OpenTable: create, probe, insert, search, delete with tombstones,
 *      then add the two rehashes.
 */

#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* ------------------------------------------------------------------ */
/* Hash functions                                                     */
/* ------------------------------------------------------------------ */

unsigned hash_string(const char *s, unsigned m)
{
    /* TODO: Horner, base 31, reduce modulo m at every step, unsigned. */
    (void)s;
    (void)m;
    return 0;
}

unsigned hash_int(int key, unsigned m)
{
    /* TODO: the division method, (unsigned)key % m. */
    (void)key;
    (void)m;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Open hashing: separate chaining                                    */
/* ------------------------------------------------------------------ */

ChainTable *chain_create(unsigned m)
{
    /* TODO: allocate the head array of m slots, all NULL. */
    (void)m;
    return NULL;
}

int chain_insert(ChainTable *t, const char *key, int value)
{
    /* TODO: replace an existing key, else push a new node; resize at alpha>1. */
    (void)t;
    (void)key;
    (void)value;
    return 0;
}

int chain_search(const ChainTable *t, const char *key, int *out)
{
    /* TODO: walk the one chain h(key) selects. */
    (void)t;
    (void)key;
    (void)out;
    return 0;
}

int chain_delete(ChainTable *t, const char *key)
{
    /* TODO: unlink the node and free its key copy. */
    (void)t;
    (void)key;
    return 0;
}

int chain_count(const ChainTable *t)
{
    (void)t;
    return 0;
}

unsigned chain_slots(const ChainTable *t)
{
    (void)t;
    return 0;
}

double chain_load_factor(const ChainTable *t)
{
    (void)t;
    return 0.0;
}

void chain_destroy(ChainTable *t)
{
    (void)t;
}

/* ------------------------------------------------------------------ */
/* Closed hashing: open addressing, linear probing, tombstones        */
/* ------------------------------------------------------------------ */

OpenTable *oa_create(unsigned m)
{
    /* TODO: allocate m slots, all EMPTY. */
    (void)m;
    return NULL;
}

int oa_insert(OpenTable *t, const char *key, int value)
{
    /* TODO: probe; replace or place; rehash on occupancy/tombstone limits. */
    (void)t;
    (void)key;
    (void)value;
    return 0;
}

int oa_search(const OpenTable *t, const char *key, int *out)
{
    /* TODO: probe until the key or EMPTY; DELETED keeps the walk alive. */
    (void)t;
    (void)key;
    (void)out;
    return 0;
}

int oa_delete(OpenTable *t, const char *key)
{
    /* TODO: find the key and mark the slot DELETED -- never clear it. */
    (void)t;
    (void)key;
    return 0;
}

int oa_count(const OpenTable *t)
{
    (void)t;
    return 0;
}

int oa_tombstones(const OpenTable *t)
{
    (void)t;
    return 0;
}

unsigned oa_slots(const OpenTable *t)
{
    (void)t;
    return 0;
}

double oa_load_factor(const OpenTable *t)
{
    (void)t;
    return 0.0;
}

void oa_destroy(OpenTable *t)
{
    (void)t;
}
