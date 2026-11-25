/*
 * test_visible.c -- the visible tests for Coding Assignment 10 (Hashing).
 *
 * Run locally to sanity-check your hash.c before submitting; the autograder
 * runs a larger hidden suite on top of these (randomised model checks,
 * tombstone stress, a load-factor measurement, and an ASan/UBSan build).
 *
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c hash.c
 *   ./test_visible
 *
 * Every expected number here was computed from the polynomial hash in the
 * header, not guessed: if a test fails, the first thing to check is your
 * hash_str, because every placement below depends on it.
 */

#include <stdio.h>
#include <string.h>

#include "hash.h"

static int failures = 0;

static void check_uint(const char *name, unsigned long got, unsigned long want)
{
    if (got == want) {
        printf("  PASS  %s\n", name);
    } else {
        printf("  FAIL  %s (got %lu, want %lu)\n", name, got, want);
        failures++;
    }
}

static void check_int(const char *name, int got, int want)
{
    check_uint(name, (unsigned long)got, (unsigned long)want);
}

static const char *KEYS[8] = { "alice", "bob", "carol", "dave",
                               "erin", "frank", "grace", "heidi" };

static void test_hash(void)
{
    printf("hash_str\n");
    check_uint("hash(\"abc\", 100)",  hash_str("abc", 100), 54);
    check_uint("hash(\"abc\", 1000)", hash_str("abc", 1000), 354);
    check_uint("hash(\"alice\", 1000)", hash_str("alice", 1000), 40);
    check_uint("hash(\"\", 7)", hash_str("", 7), 0);
    check_uint("hash(\"zoe\", 11)", hash_str("zoe", 11), 4);
}

static void test_chaining(void)
{
    printf("separate chaining, m = 7\n");
    ChainTable t;
    check_int("chain_init", chain_init(&t, 7), 0);

    int inserted = 1;
    for (int i = 0; i < 8; i++)
        if (chain_put(&t, KEYS[i], i + 1) != 1)
            inserted = 0;
    check_int("8 inserts return 1", inserted, 1);
    check_uint("n == 8", t.n, 8);

    check_int("get alice", chain_get(&t, "alice", &inserted), 1);
    check_int("alice value", inserted, 1);
    int absent = 0;
    check_int("get sybil (absent)", chain_get(&t, "sybil", &absent), 0);

    /* alice, erin, heidi all hash to slot 6; each insert is pushed on the
       head, so the newest key is cheapest to find. */
    check_uint("probes heidi (head)", chain_probes(&t, "heidi"), 1);
    check_uint("probes erin (middle)", chain_probes(&t, "erin"), 2);
    check_uint("probes alice (tail)", chain_probes(&t, "alice"), 3);
    check_uint("probes sybil (absent, slot 6)", chain_probes(&t, "sybil"), 3);

    check_int("update alice returns 0", chain_put(&t, "alice", 50), 0);
    check_uint("n still 8 after update", t.n, 8);
    check_int("alice updated", chain_get(&t, "alice", &absent), 1);
    check_int("alice new value", absent, 50);

    check_int("delete carol", chain_del(&t, "carol"), 1);
    check_int("carol gone", chain_get(&t, "carol", &absent), 0);
    check_int("delete carol again", chain_del(&t, "carol"), 0);
    check_uint("n == 7", t.n, 7);

    chain_free(&t);
    check_uint("chain_free zeroes n", t.n, 0);
    check_uint("chain_free zeroes m", t.m, 0);
}

static void test_open_addressing(void)
{
    printf("open addressing, m = 11, linear probing\n");
    OpenTable t;
    check_int("oa_init", oa_init(&t, 11), 0);

    int inserted = 1;
    for (int i = 0; i < 8; i++)
        if (oa_put(&t, KEYS[i], i + 1) != 1)
            inserted = 0;
    check_int("8 inserts return 1", inserted, 1);
    check_uint("n == 8", t.n, 8);

    for (int i = 0; i < 8; i++) {
        int v = -1;
        check_int("get stored key", oa_get(&t, KEYS[i], &v), 1);
        check_int("stored value", v, i + 1);
    }

    /* alice -> 10 (1 probe); grace -> 4,5,6,7 (4); heidi -> 4..8 (5);
       sybil -> 7, 8, then slot 9 empty (3, counting the empty slot). */
    check_uint("probes alice", oa_probes(&t, "alice"), 1);
    check_uint("probes grace", oa_probes(&t, "grace"), 4);
    check_uint("probes heidi", oa_probes(&t, "heidi"), 5);
    check_uint("probes sybil (absent)", oa_probes(&t, "sybil"), 3);

    check_int("update erin returns 0", oa_put(&t, "erin", 55), 0);
    check_uint("n unchanged by update", t.n, 8);

    /* Delete bob from slot 4. The slot becomes a tombstone: grace (which was
       displaced past it) must still be findable, and a miss must walk on. */
    check_int("delete bob", oa_del(&t, "bob"), 1);
    check_uint("n == 7", t.n, 7);
    int v = -1;
    check_int("grace still found after tombstone", oa_get(&t, "grace", &v), 1);
    check_int("grace value", v, 7);
    check_uint("probes grace after tombstone", oa_probes(&t, "grace"), 4);
    check_uint("probes bob (absent, walks past tombstone)",
               oa_probes(&t, "bob"), 6);

    /* zoe hashes to 4: it must REUSE the tombstone, not append to the run. */
    check_int("insert zoe reuses tombstone", oa_put(&t, "zoe", 99), 1);
    check_uint("probes zoe (reused slot)", oa_probes(&t, "zoe"), 1);
    check_uint("n == 8 again", t.n, 8);

    /* An update must be found across a tombstone, never duplicated. */
    check_int("delete grace", oa_del(&t, "grace"), 1);
    check_int("update heidi across tombstone", oa_put(&t, "heidi", 88), 0);
    check_uint("n unchanged by that update", t.n, 7);
    check_int("heidi value", oa_get(&t, "heidi", &v), 1);
    check_int("heidi new value", v, 88);

    oa_free(&t);
    check_uint("oa_free zeroes m", t.m, 0);
}

static void test_full_table(void)
{
    printf("full-table semantics, m = 3\n");
    OpenTable t;
    check_int("oa_init", oa_init(&t, 3), 0);
    check_int("put a", oa_put(&t, "a", 1), 1);
    check_int("put b", oa_put(&t, "b", 2), 1);
    check_int("put c", oa_put(&t, "c", 3), 1);
    check_int("put into full table is refused", oa_put(&t, "d", 4), -1);
    check_int("delete b", oa_del(&t, "b"), 1);
    check_int("put d into the tombstone", oa_put(&t, "d", 4), 1);
    int v = -1;
    check_int("d is present", oa_get(&t, "d", &v), 1);
    check_int("d value", v, 4);
    oa_free(&t);
}

int main(void)
{
    printf("=== Coding Assignment 10 -- visible tests ===\n");
    test_hash();
    test_chaining();
    test_open_addressing();
    test_full_table();

    if (failures == 0) {
        printf("\nAll visible tests passed.\n");
        return 0;
    }
    printf("\n%d visible test(s) FAILED.\n", failures);
    return 1;
}
