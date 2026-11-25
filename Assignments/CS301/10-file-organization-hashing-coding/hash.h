/*
 * hash.h -- the contract for Coding Assignment 10 (File Organization and
 * Hashing), Week 10's institute practical: hash-table construction and
 * collision handling.
 *
 * This header is given to you; do NOT change it. Your job is to provide the
 * functions below in hash.c (start from starter_hash.c).
 *
 * Setting (the running calculator, Weeks 3-11). Week 9 sorted the calculator's
 * symbol table once and searched it in Theta(log n). Week 10 replaces it with a
 * hash table: slot = h(name), one probe expected, and the price is that the
 * table no longer keeps the names in order. You build the same table twice,
 * once with each collision policy from the deck:
 *
 *   ChainTable -- open hashing / SEPARATE CHAINING (Section 3 of the deck).
 *                 Colliding entries live in a per-slot linked list.
 *   OpenTable  -- closed hashing / OPEN ADDRESSING (Section 4), using
 *                 LINEAR PROBING with tombstones.
 *
 * Both tables store (key, value) pairs where the key is a C string of any
 * length (including the empty string ""), and the value is an int. Both must
 * behave the same way for the operations they share:
 *
 *   DUPLICATE POLICY. The key is unique in the table. Inserting a key that is
 *   already present REPLACES its value and leaves the count unchanged. The
 *   replacement returns 0; a genuinely new key returns 1.
 *
 *   KEY OWNERSHIP. The table stores its own private copy of every key. The
 *   caller's buffer may be a stack array, may be overwritten, and may go out
 *   of scope after the call -- the table must still find the key afterwards.
 *   Every copy is released by delete, by rehash (the copy is moved, not
 *   duplicated) and by destroy. No key bytes may leak.
 *
 *   FAILURE. create returns NULL when it cannot allocate (and when m == 0).
 *   insert returns -1 when it cannot allocate. Every other function is
 *   NULL-safe on a NULL table: search/delete return 0, count/slots return 0,
 *   load_factor returns 0.0, destroy does nothing. A NULL key is never passed
 *   to you; a NULL out pointer in search means "found or not, do not report
 *   the value".
 *
 * The deck's C is at Slides/CS301/10-file-organization-hashing.tex:
 * hash functions (Section 2), separate chaining (Section 3), open addressing,
 * linear probing and the tombstone rule (Section 4). The worked examples in
 * the problem statement are fresh data to check against.
 */

#ifndef HASH_H
#define HASH_H

/* ------------------------------------------------------------------ */
/* Hash functions (Section 2 of the deck)                             */
/* ------------------------------------------------------------------ */

/*
 * hash_string: hash the NUL-terminated string s into [0, m-1] with the deck's
 * polynomial (Horner) scheme -- base B = 31, reduced modulo m at EVERY step,
 * unsigned arithmetic:
 *
 *     unsigned h = 0;
 *     for each byte c of s:  h = (h * 31u + (unsigned char)c) % m;
 *     return h;
 *
 * The reduction inside the loop is what keeps 31^len from overflowing; do not
 * "optimise" it into one final modulo. hash_string("", m) is 0.
 * Precondition: s != NULL, m >= 1. Undefined if m == 0.
 */
unsigned hash_string(const char *s, unsigned m);

/*
 * hash_int: the division method for an integer key, h(k) = k mod m, with the
 * key reinterpreted as unsigned (two's complement) so that negative keys stay
 * in range: return (unsigned)key % m.
 * Precondition: m >= 1. Undefined if m == 0.
 *
 * Worked values with m = 13: 40 -> 1, 53 -> 1, 66 -> 1, 100 -> 9, -1 -> 8.
 */
unsigned hash_int(int key, unsigned m);

/* ------------------------------------------------------------------ */
/* Open hashing: separate chaining                                    */
/* ------------------------------------------------------------------ */

typedef struct ChainTable ChainTable;

/*
 * chain_create: make an empty table with exactly m slots.
 *   - returns NULL if m == 0 or allocation fails;
 *   - no rehash happens here, so chain_slots(chain_create(m)) == m.
 */
ChainTable *chain_create(unsigned m);

/*
 * chain_insert: insert (key, value), or replace value if key is present.
 *   - returns 1 if a new key was inserted, 0 if an existing key's value was
 *     replaced (count unchanged), -1 on allocation failure;
 *   - OWNS A COPY of key (see KEY OWNERSHIP above);
 *   - RESIZE RULE: if inserting a new key would make n > m (that is, the load
 *     factor alpha = n/m would exceed 1.0), the table is first rehashed into
 *     a larger slot array. Consequently, after any insert that returns 1,
 *     chain_count(t) <= (int)chain_slots(t). Any growth factor >= 2 is fine.
 *   - expected O(1 + alpha).
 */
int chain_insert(ChainTable *t, const char *key, int value);

/*
 * chain_search: look up key.
 *   - returns 1 and stores the value in *out (when out != NULL) if present,
 *     0 if absent;
 *   - must not modify the table.
 */
int chain_search(const ChainTable *t, const char *key, int *out);

/*
 * chain_delete: remove key.
 *   - returns 1 if it was present and is now gone, 0 if it was absent;
 *   - frees the stored key copy;
 *   - expected O(1 + alpha).
 */
int chain_delete(ChainTable *t, const char *key);

/* Number of live (key, value) pairs; 0 for a NULL table. */
int chain_count(const ChainTable *t);

/* Current number of slots m (it changes after a rehash); 0 for NULL. */
unsigned chain_slots(const ChainTable *t);

/* n/m as a double; 0.0 for a NULL table. Never exceeds 1.0 after an insert. */
double chain_load_factor(const ChainTable *t);

/* Release the table, every chain node and every stored key copy. NULL-safe. */
void chain_destroy(ChainTable *t);

/* ------------------------------------------------------------------ */
/* Closed hashing: open addressing with linear probing + tombstones    */
/* ------------------------------------------------------------------ */

typedef struct OpenTable OpenTable;

/*
 * oa_create: make an empty table with exactly m slots, all EMPTY.
 *   - returns NULL if m == 0 or allocation fails;
 *   - oa_slots(oa_create(m)) == m and oa_tombstones(...) == 0.
 *
 * Each slot is in one of three states: EMPTY, OCCUPIED, DELETED.
 *   - a search stops only at EMPTY; DELETED is "keep probing";
 *   - an insert may reuse the first DELETED slot it passes.
 */
OpenTable *oa_create(unsigned m);

/*
 * oa_insert: insert (key, value), or replace value if key is present.
 *   - returns 1 if a new key was inserted, 0 if an existing key's value was
 *     replaced, -1 on allocation failure;
 *   - OWNS A COPY of key;
 *   - LINEAR PROBING: the probe sequence for key is
 *     h, h+1, h+2, ... (mod m) with h = hash_string(key, m);
 *   - RESIZE RULE. After any insert that returns 1 (a new key), both of these
 *     must hold:
 *         (a) occupancy:  n + tombstones <= m/2   (so 2*n <= m);
 *         (b) tombstones: tombstones   <= m/4.
 *     Reach them by rehashing: grow the slot array when (a) would be broken,
 *     and rebuild the table (same m is enough) when (b) would be broken. Both
 *     rehashes are what keep a linear-probing table from degrading.
 *   - expected O(1 + 1/(1-alpha)) probes.
 */
int oa_insert(OpenTable *t, const char *key, int value);

/*
 * oa_search: look up key with the SAME probe sequence.
 *   - returns 1 and stores the value in *out (when out != NULL) if present,
 *     0 if absent;
 *   - stops at EMPTY, continues past DELETED, stops at m probes;
 *   - must not modify the table.
 */
int oa_search(const OpenTable *t, const char *key, int *out);

/*
 * oa_delete: remove key.
 *   - returns 1 if it was present and is now gone, 0 if it was absent;
 *   - MUST NOT clear the slot: mark it DELETED (a tombstone), increment the
 *     tombstone count, free the stored key copy, decrement the count;
 *   - clearing the slot hides every key that probed past it -- the hidden
 *     suite tests exactly that;
 *   - expected O(1 + 1/(1-alpha)) probes.
 */
int oa_delete(OpenTable *t, const char *key);

/* Number of live (key, value) pairs; 0 for NULL. */
int oa_count(const OpenTable *t);

/* Number of DELETED slots currently in the table; 0 for NULL. */
int oa_tombstones(const OpenTable *t);

/* Current number of slots m (it changes after a rehash); 0 for NULL. */
unsigned oa_slots(const OpenTable *t);

/* n/m as a double (live entries per slot); 0.0 for NULL. Never exceeds 0.5
 * after an insert that returned 1. */
double oa_load_factor(const OpenTable *t);

/* Release the table, every stored key copy and the slot array. NULL-safe. */
void oa_destroy(OpenTable *t);

#endif /* HASH_H */
