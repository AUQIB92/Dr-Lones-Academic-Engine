/*
 * test_visible.c -- the visible tests, one per worked example in the Week 10
 * coding problem statement. Run locally to sanity-check your hash.c before
 * submitting; the autograder runs a LARGER hidden suite on top of these.
 *
 * Build + run (from the assignment directory):
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c hash.c
 *   ./test_visible
 */

#include <stdio.h>
#include <string.h>
#include "hash.h"

static int total = 0;
static int failures = 0;

static void check(const char *name, int ok)
{
    total++;
    if (ok) {
        printf("  PASS  %s\n", name);
    } else {
        printf("  FAIL  %s\n", name);
        failures++;
    }
}

static void check_uint(const char *name, unsigned got, unsigned want)
{
    total++;
    if (got == want) {
        printf("  PASS  %s (%u)\n", name, got);
    } else {
        printf("  FAIL  %s (got %u, want %u)\n", name, got, want);
        failures++;
    }
}

static void check_int(const char *name, int got, int want)
{
    total++;
    if (got == want) {
        printf("  PASS  %s (%d)\n", name, got);
    } else {
        printf("  FAIL  %s (got %d, want %d)\n", name, got, want);
        failures++;
    }
}

static void check_double(const char *name, double got, double want)
{
    double d = got - want;
    if (d < 0.0) d = -d;
    total++;
    if (d < 1e-9) {
        printf("  PASS  %s (%.4f)\n", name, got);
    } else {
        printf("  FAIL  %s (got %.4f, want %.4f)\n", name, got, want);
        failures++;
    }
}

int main(void)
{
    printf("Example 1: the two hash functions\n");
    check_uint("hash_int(40, 13)",  hash_int(40, 13),  1);
    check_uint("hash_int(53, 13)",  hash_int(53, 13),  1);
    check_uint("hash_int(66, 13)",  hash_int(66, 13),  1);
    check_uint("hash_int(100, 13)", hash_int(100, 13), 9);
    check_uint("hash_int(-1, 13)",  hash_int(-1, 13),  8);
    check_uint("hash_string(\"dog\", 101)",   hash_string("dog", 101),   58);
    check_uint("hash_string(\"\", 101)",      hash_string("", 101),      0);
    check_uint("hash_string(\"alpha\", 11)",  hash_string("alpha", 11),  2);
    check_uint("hash_string(\"beta\", 11)",   hash_string("beta", 11),   2);
    check_uint("hash_string(\"gamma\", 11)",  hash_string("gamma", 11),  10);
    check_uint("hash_string(\"delta\", 11)",  hash_string("delta", 11),  0);

    printf("Example 2: chaining -- insert, search, replace, delete\n");
    {
        ChainTable *t = chain_create(5);
        check("chain_create(5) returned a table", t != NULL);
        if (t) {
            check_uint("chain_slots == 5", chain_slots(t), 5);
            check_int("empty table count == 0", chain_count(t), 0);
            check_double("empty table load factor == 0", chain_load_factor(t), 0.0);
            check_int("search miss on empty table", chain_search(t, "alpha", NULL), 0);
            check_int("delete absent on empty table", chain_delete(t, "alpha"), 0);

            check_int("insert alpha=10", chain_insert(t, "alpha", 10), 1);
            check_int("insert beta=20",  chain_insert(t, "beta", 20),  1);
            check_int("insert gamma=30", chain_insert(t, "gamma", 30), 1);
            check_int("insert delta=40", chain_insert(t, "delta", 40), 1);
            check_int("count == 4", chain_count(t), 4);
            check_double("load factor == 4/5", chain_load_factor(t), 0.8);

            /* beta and delta share bucket 2; both must be found. */
            {
                int v = 0;
                check("search beta finds 20", chain_search(t, "beta", &v) == 1 && v == 20);
                v = 0;
                check("search delta finds 40", chain_search(t, "delta", &v) == 1 && v == 40);
            }
            check_int("replace beta with 25 returns 0", chain_insert(t, "beta", 25), 0);
            check_int("count unchanged after replace", chain_count(t), 4);
            {
                int v = 0;
                check("replace updated beta's value",
                      chain_search(t, "beta", &v) == 1 && v == 25);
            }
            check_int("delete alpha returns 1", chain_delete(t, "alpha"), 1);
            check_int("delete alpha again returns 0", chain_delete(t, "alpha"), 0);
            check_int("search alpha after delete", chain_search(t, "alpha", NULL), 0);
            check_int("count == 3", chain_count(t), 3);
            {
                int v = 0;
                check("delta survives delete of a different key",
                      chain_search(t, "delta", &v) == 1 && v == 40);
            }
            check_int("reinsert alpha=99", chain_insert(t, "alpha", 99), 1);
            {
                int v = 0;
                check("reinserted alpha finds 99",
                      chain_search(t, "alpha", &v) == 1 && v == 99);
            }
            /* The table owns a copy of the key: mutate the caller's buffer. */
            {
                char buf[32] = "ephemeral";
                check_int("insert from a mutable buffer",
                          chain_insert(t, buf, 7), 1);
                strcpy(buf, "overwritten");
                check_int("stored key survives the caller's buffer changing",
                          chain_search(t, "ephemeral", NULL), 1);
                check_int("the changed buffer is not a key",
                          chain_search(t, "overwritten", NULL), 0);
            }
            chain_destroy(t);
        }
    }

    printf("Example 3: open addressing -- linear probing and the tombstone\n");
    {
        /* m = 11: alpha,beta hash to 2; gamma to 10; delta to 0. Four live
         * entries in 11 slots keeps occupancy at 8 <= 11, so no rehash. */
        OpenTable *t = oa_create(11);
        check("oa_create(11) returned a table", t != NULL);
        if (t) {
            check_uint("oa_slots == 11", oa_slots(t), 11);
            check_int("empty table count == 0", oa_count(t), 0);
            check_int("empty table tombstones == 0", oa_tombstones(t), 0);
            check_int("search miss on empty table", oa_search(t, "alpha", NULL), 0);

            check_int("insert alpha=10", oa_insert(t, "alpha", 10), 1);
            check_int("insert beta=20",  oa_insert(t, "beta", 20),  1);
            check_int("insert gamma=30", oa_insert(t, "gamma", 30), 1);
            check_int("insert delta=40", oa_insert(t, "delta", 40), 1);
            check_int("count == 4", oa_count(t), 4);
            check_double("load factor == 4/11", oa_load_factor(t), 4.0 / 11.0);

            check_int("delete alpha (home slot 2)", oa_delete(t, "alpha"), 1);
            check_int("the deleted slot is a tombstone", oa_tombstones(t), 1);
            check_int("count == 3", oa_count(t), 3);
            {
                int v = 0;
                check("beta probed past alpha's tombstone and is still found",
                      oa_search(t, "beta", &v) == 1 && v == 20);
            }
            check_int("delete absent key returns 0", oa_delete(t, "alpha"), 0);
            check_int("reinsert alpha=99 reuses the tombstone",
                      oa_insert(t, "alpha", 99), 1);
            check_int("reinsert raised no new tombstone", oa_tombstones(t), 0);
            {
                int v = 0;
                check("reinserted alpha finds 99",
                      oa_search(t, "alpha", &v) == 1 && v == 99);
            }
            {
                char buf[32] = "ephemeral";
                check_int("insert from a mutable buffer",
                          oa_insert(t, buf, 7), 1);
                strcpy(buf, "overwritten");
                check_int("stored key survives the caller's buffer changing",
                          oa_search(t, "ephemeral", NULL), 1);
            }
            check_int("replace gamma with 31 returns 0",
                      oa_insert(t, "gamma", 31), 0);
            check_int("count unchanged after replace", oa_count(t), 5);
            oa_destroy(t);
        }
    }

    printf("Example 4: m = 1 -- the smallest legal table\n");
    {
        ChainTable *c = chain_create(1);
        OpenTable  *o = oa_create(1);
        check("chain_create(1) and oa_create(1) both succeed", c != NULL && o != NULL);
        if (c && o) {
            char key[16];
            int ok = 1;
            for (int i = 0; i < 12; i++) {
                snprintf(key, sizeof key, "k%d", i);
                if (chain_insert(c, key, i) != 1) ok = 0;
                if (oa_insert(o, key, i) != 1) ok = 0;
            }
            check("12 inserts into a 1-slot table", ok);
            check_int("chain count == 12", chain_count(c), 12);
            check_int("oa count == 12", oa_count(o), 12);
            ok = 1;
            for (int i = 0; i < 12; i++) {
                int v = -1;
                snprintf(key, sizeof key, "k%d", i);
                if (chain_search(c, key, &v) != 1 || v != i) ok = 0;
                if (oa_search(o, key, &v) != 1 || v != i) ok = 0;
            }
            check("all 12 keys found in both tables", ok);
            check("chaining grew past one slot", chain_slots(c) > 1);
            check("open addressing grew past one slot", oa_slots(o) > 1);
        }
        chain_destroy(c);
        oa_destroy(o);
    }

    printf("Example 5: NULL-safe teardown and a rejected m == 0\n");
    chain_destroy(NULL);
    oa_destroy(NULL);
    check("destroy(NULL) is safe", 1);
    check("chain_create(0) returns NULL", chain_create(0) == NULL);
    check("oa_create(0) returns NULL", oa_create(0) == NULL);

    printf("\n");
    if (failures == 0) {
        printf("All %d visible tests passed.\n", total);
        return 0;
    }
    printf("%d of %d visible test(s) FAILED.\n", failures, total);
    return 1;
}
