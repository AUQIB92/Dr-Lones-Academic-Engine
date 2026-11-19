/*
 * hash.h -- the contract for Coding Assignment 10 (File Organization &
 * Hashing), the instructor-supplied Week 10 lab: hash-table construction and
 * collision handling.
 *
 * This header is given to you; do NOT change it. Your job is to implement the
 * twelve functions below in hash.c (start from starter_hash.c).
 *
 * Setting (the running calculator, Weeks 3-10). Since Week 6 the calculator
 * has kept its identifier table as a linked list, and every token lookup has
 * cost up to n comparisons. This module is the replacement: the SAME
 * symbol-table ADT -- put / get / delete, key is a C string -- built twice,
 * once with each collision family from the lecture:
 *
 *   ChainTable  open hashing   = separate chaining   (a list per slot)
 *   OpenTable   closed hashing = open addressing     (linear probing,
 *                                                    with tombstones)
 *
 * Both tables are FIXED SIZE for grading: init allocates m slots, and no
 * function ever resizes. Load-factor policy (rehashing) is a design question
 * on the written assignment, not code here.
 *
 * Keys are NUL-terminated strings; the table COPIES each key, so the caller
 * may free or reuse its buffer immediately after a successful put.
 *
 * Return conventions (identical in both tables):
 *   put :  1  inserted a new entry
 *          0  an entry for this key already existed and its value was updated
 *         -1  could not insert (allocation failure, or open table full)
 *   get :  1  key present, *out receives the value
 *          0  key absent (*out untouched)
 *   del :  1  key was present and has been removed
 *          0  key absent, table unchanged
 *
 * Every function that takes a table pointer requires it to have been
 * initialised by the matching init, and init requires m > 0.
 */

#ifndef HASH_H
#define HASH_H

#include <stddef.h>

/* -------------------------------------------------------------------------
 * hash_str -- the polynomial (rolling) hash from the lecture.
 *
 *   h = 0; for each character c of s: h = (h * 31 + c) mod m
 *
 * Requirements:
 *   - the accumulation must use UNSIGNED arithmetic and the character must be
 *     cast to unsigned char before it enters the sum (a plain char may be
 *     signed, and a byte above 127 would then contribute a negative value --
 *     the bug the lecture calls out);
 *   - returns a value in [0, m-1]; m > 0;
 *   - cost O(length of s), one pass, no recursion, no allocation;
 *   - the empty string hashes to 0.
 * ------------------------------------------------------------------------- */
unsigned long hash_str(const char *s, unsigned long m);

/* -------------------------------------------------------------------------
 * ChainTable -- open hashing (separate chaining).
 *
 *   slots[i] is the head of the list of every entry whose hash is i.
 *   The slot array itself is allocated by chain_init.
 * ------------------------------------------------------------------------- */

typedef struct HNode {
    char         *key;   /* owned copy of the key */
    int           val;
    struct HNode *next;
} HNode;

typedef struct {
    HNode  **slots;      /* slots[0..m-1], each NULL when the chain is empty */
    size_t   m;          /* number of slots */
    size_t   n;          /* number of stored entries */
} ChainTable;

/*
 * chain_init: allocate m slot pointers, all NULL; n = 0.
 *   returns 0 on success, -1 if m == 0 or the allocation fails (the table is
 *   left in a state that chain_free can safely be called on).
 */
int chain_init(ChainTable *t, size_t m);

/*
 * chain_put: insert (key, val), or update the value if the key is present.
 *   - the key must be COPIED (the caller retains ownership of its buffer);
 *   - an update must NOT add a second node for the same key;
 *   - new nodes may be pushed at the head (O(1)) -- probe order is then
 *     newest-first, which chain_probes below must match;
 *   - returns 1 / 0 / -1 as documented at the top of this file.
 */
int chain_put(ChainTable *t, const char *key, int val);

/*
 * chain_get: 1 and *out = value if present, else 0.
 */
int chain_get(const ChainTable *t, const char *key, int *out);

/*
 * chain_del: unlink and free the node for key; 1 if it was present, else 0.
 */
int chain_del(ChainTable *t, const char *key);

/*
 * chain_probes: how many stored keys are compared before the key is found,
 * or -- when the key is absent -- the length of its chain (the number of
 * nodes examined before the walk reaches NULL). Counting starts at 1 for the
 * head node; an absent key in an empty chain costs 0.
 *   - must not modify the table; cost O(chain length).
 */
size_t chain_probes(const ChainTable *t, const char *key);

/*
 * chain_free: release every node and every copied key, then the slot array,
 * and zero the struct. Safe on a table whose init failed.
 */
void chain_free(ChainTable *t);

/* -------------------------------------------------------------------------
 * OpenTable -- closed hashing (open addressing, linear probing, tombstones).
 *
 *   Every entry lives in a slot of the array itself. A slot is in exactly one
 *   of three states:
 *     EMPTY     never held an entry (or holds none now and can stop a search)
 *     OCCUPIED  holds a live entry
 *     DELETED   holds no entry but must NOT stop a search (a tombstone)
 *
 *   n      counts LIVE entries,
 *   used   counts OCCUPIED + DELETED slots (everything that is not EMPTY),
 *          which is what decides whether a free slot still exists.
 * ------------------------------------------------------------------------- */

#define OA_EMPTY   0
#define OA_OCCUPIED 1
#define OA_DELETED 2

typedef struct {
    char         *key;    /* owned copy, NULL unless OCCUPIED */
    int           val;
    unsigned char state;  /* OA_EMPTY / OA_OCCUPIED / OA_DELETED */
} OASlot;

typedef struct {
    OASlot *slots;        /* slots[0..m-1] */
    size_t  m;
    size_t  n;            /* live entries */
    size_t  used;         /* non-empty slots (live + tombstones) */
} OpenTable;

/*
 * oa_init: allocate m slots, all OA_EMPTY; n = used = 0.
 *   returns 0 on success, -1 on m == 0 or allocation failure.
 */
int oa_init(OpenTable *t, size_t m);

/*
 * oa_put: insert with linear probing, starting at hash_str(key, m).
 *   - if the key is already present, update its value and return 0;
 *   - otherwise insert at the FIRST EMPTY slot reachable, but REUSE an
 *     earlier tombstone if the walk passed one (keep the index of the first
 *     DELETED slot seen and place the new entry there);
 *   - a tombstone does NOT stop the walk: an existing key further along must
 *     still be found and updated, not duplicated;
 *   - returns 1 on insert, 0 on update, -1 if the table has no usable slot
 *     (every slot OCCUPIED, or every remaining slot DELETED with no key match).
 */
int oa_put(OpenTable *t, const char *key, int val);

/*
 * oa_get: probe from hash_str(key, m); 1 and *out = value on a match.
 * A DELETED slot is walked over; the search stops at the first EMPTY slot
 * (absent, return 0) or when every slot has been examined (absent, return 0).
 */
int oa_get(const OpenTable *t, const char *key, int *out);

/*
 * oa_del: on a match, free the key, set the slot to OA_DELETED, decrement n
 * (used is unchanged -- the slot is still not EMPTY), and return 1; else 0.
 */
int oa_del(OpenTable *t, const char *key);

/*
 * oa_probes: number of SLOT INSPECTIONS a get for this key performs.
 *   - on a hit: slots examined up to and including the matching slot;
 *   - on a miss: slots examined up to and including the terminating EMPTY
 *     slot (so a miss in a table whose first probe is empty costs 1);
 *   - if the table is full and the key is absent, every slot is examined.
 *   - must not modify the table.
 */
size_t oa_probes(const OpenTable *t, const char *key);

/*
 * oa_free: free every owned key, then the slot array, and zero the struct.
 * Safe on a table whose init failed.
 */
void oa_free(OpenTable *t);

#endif /* HASH_H */
