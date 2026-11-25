/*
 * test_visible.c -- the worked examples from the problem statement.
 * These are the tests you can run yourself; the hidden suite is larger.
 *
 * Build:
 *   gcc -std=c11 -Wall -Wextra -Werror -o test_visible test_visible.c tree.c
 *   ./test_visible
 */

#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

static int failures = 0;
static int checks = 0;

#define CHECK(cond, msg)                                                   \
    do {                                                                   \
        checks++;                                                          \
        if (!(cond)) {                                                     \
            failures++;                                                    \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, (msg));         \
        }                                                                  \
    } while (0)

#define CHECK_ARR(got, want, n, msg)                                       \
    do {                                                                   \
        checks++;                                                          \
        int ok = 1;                                                        \
        for (size_t i_ = 0; i_ < (size_t)(n); i_++)                        \
            if ((got)[i_] != (want)[i_]) ok = 0;                           \
        if (!ok) {                                                         \
            failures++;                                                    \
            printf("FAIL %s:%d: %s\n  got: ", __FILE__, __LINE__, (msg));  \
            for (size_t i_ = 0; i_ < (size_t)(n); i_++)                    \
                printf("%d ", (got)[i_]);                                  \
            printf("\n  want: ");                                          \
            for (size_t i_ = 0; i_ < (size_t)(n); i_++)                    \
                printf("%d ", (want)[i_]);                                 \
            printf("\n");                                                  \
        }                                                                  \
    } while (0)

int main(void) {
    Tree *t = tree_create();
    CHECK(t != NULL, "tree_create");
    CHECK(tree_size(t) == 0, "empty size");
    CHECK(tree_height(t) == -1, "empty height");
    CHECK(tree_search(t, 1) == 0, "empty search");
    CHECK(tree_min(t, NULL) == 0, "empty min");

    int keys[] = {9, 5, 12, 2, 7, 11, 15, 3, 6};
    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; i++)
        CHECK(tree_insert(t, keys[i]) == 1, "insert returns 1");

    int want_in[]  = {2, 3, 5, 6, 7, 9, 11, 12, 15};
    int want_pre[] = {9, 5, 2, 3, 7, 6, 12, 11, 15};
    int want_post[] = {3, 2, 6, 7, 5, 11, 15, 12, 9};
    int want_lvl[] = {9, 5, 12, 2, 7, 11, 15, 3, 6};
    int buf[16];

    CHECK(tree_size(t) == 9, "size after nine inserts");
    CHECK(tree_height(t) == 3, "height of the worked tree");
    CHECK(tree_inorder(t, buf, 16) == 9, "inorder count");
    CHECK_ARR(buf, want_in, 9, "inorder");
    CHECK(tree_preorder(t, buf, 16) == 9, "preorder count");
    CHECK_ARR(buf, want_pre, 9, "preorder");
    CHECK(tree_postorder(t, buf, 16) == 9, "postorder count");
    CHECK_ARR(buf, want_post, 9, "postorder");
    CHECK(tree_levelorder(t, buf, 16) == 9, "level-order count");
    CHECK_ARR(buf, want_lvl, 9, "level order");
    CHECK(tree_inorder(t, buf, 4) == 4, "inorder truncates to cap");
    CHECK_ARR(buf, want_in, 4, "inorder first four");

    int v = 0;
    CHECK(tree_min(t, &v) == 1 && v == 2, "min");
    CHECK(tree_max(t, &v) == 1 && v == 15, "max");
    CHECK(tree_search(t, 6) == 1, "search hit");
    CHECK(tree_search(t, 8) == 0, "search miss");

    /* Delete a two-child node: successor 6 replaces 5. */
    CHECK(tree_delete(t, 5) == 1, "delete 5");
    int want_del[] = {2, 3, 6, 7, 9, 11, 12, 15};
    CHECK(tree_size(t) == 8, "size after delete");
    CHECK(tree_inorder(t, buf, 16) == 8, "inorder after delete");
    CHECK_ARR(buf, want_del, 8, "inorder after deleting 5");
    CHECK(tree_delete(t, 5) == 0, "delete absent");

    /* Delete the root: successor 11 replaces 9. */
    CHECK(tree_delete(t, 9) == 1, "delete root");
    int want_root[] = {2, 3, 6, 7, 11, 12, 15};
    CHECK(tree_inorder(t, buf, 16) == 7, "inorder after deleting root");
    CHECK_ARR(buf, want_root, 7, "inorder after deleting root");

    /* Duplicates. */
    Tree *d = tree_create();
    CHECK(tree_insert(d, 4) == 1, "duplicate insert 1");
    CHECK(tree_insert(d, 4) == 1, "duplicate insert 2");
    CHECK(tree_insert(d, 4) == 1, "duplicate insert 3");
    CHECK(tree_count(d, 4) == 3, "count duplicates");
    CHECK(tree_delete(d, 4) == 1, "delete one duplicate");
    CHECK(tree_count(d, 4) == 2, "count after one delete");
    CHECK(tree_remove_all(d, 4) == 2, "remove remaining duplicates");
    CHECK(tree_count(d, 4) == 0 && tree_size(d) == 0, "duplicates gone");
    tree_destroy(d);

    /* Tree sort and range. */
    int in[]  = {4, -1, 4, 3, 0, -1, 9};
    int out[7];
    int want_sort[] = {-1, -1, 0, 3, 4, 4, 9};
    CHECK(tree_sort(in, 7, out, 7) == 7, "tree_sort count");
    CHECK_ARR(out, want_sort, 7, "tree_sort order");

    Tree *r = tree_create();
    for (size_t i = 0; i < 7; i++) tree_insert(r, in[i]);
    int range[7];
    int want_range[] = {3, 4, 4, 9};
    CHECK(tree_range(r, 3, 9, range, 7) == 4, "range count");
    CHECK_ARR(range, want_range, 4, "range [3,9]");
    CHECK(tree_range(r, 10, 20, range, 7) == 0, "empty range");
    tree_destroy(r);

    /* NULL safety. */
    CHECK(tree_search(NULL, 1) == 0, "NULL search");
    CHECK(tree_size(NULL) == 0, "NULL size");
    CHECK(tree_height(NULL) == -1, "NULL height");
    CHECK(tree_insert(NULL, 1) == -1, "NULL insert");
    CHECK(tree_delete(NULL, 1) == 0, "NULL delete");
    tree_destroy(NULL);

    tree_destroy(t);

    printf("%d checks, %d failure(s)\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
