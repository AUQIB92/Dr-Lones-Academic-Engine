/*
 * starter_tree.c -- copy this file to tree.c and implement tree.h.
 *
 * The stubs compile but fail every test. Work in this order:
 *   1. create / destroy / insert / search,
 *   2. the four traversals and height / size / min / max,
 *   3. delete (all three cases) and remove_all,
 *   4. tree_sort and tree_range.
 *
 * Duplicate policy: equal keys go in the RIGHT subtree.
 */

#include "tree.h"

#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

struct Tree {
    Node *root;
    size_t n;
};

Tree *tree_create(void) {
    /* TODO */
    return NULL;
}

void tree_destroy(Tree *t) {
    (void)t;
}

int tree_insert(Tree *t, int key) {
    (void)t;
    (void)key;
    return -1;
}

int tree_delete(Tree *t, int key) {
    (void)t;
    (void)key;
    return 0;
}

size_t tree_remove_all(Tree *t, int key) {
    (void)t;
    (void)key;
    return 0;
}

int tree_search(const Tree *t, int key) {
    (void)t;
    (void)key;
    return 0;
}

size_t tree_count(const Tree *t, int key) {
    (void)t;
    (void)key;
    return 0;
}

size_t tree_size(const Tree *t) {
    (void)t;
    return 0;
}

int tree_height(const Tree *t) {
    (void)t;
    return -1;
}

int tree_min(const Tree *t, int *out) {
    (void)t;
    (void)out;
    return 0;
}

int tree_max(const Tree *t, int *out) {
    (void)t;
    (void)out;
    return 0;
}

size_t tree_inorder(const Tree *t, int *out, size_t cap) {
    (void)t;
    (void)out;
    (void)cap;
    return 0;
}

size_t tree_preorder(const Tree *t, int *out, size_t cap) {
    (void)t;
    (void)out;
    (void)cap;
    return 0;
}

size_t tree_postorder(const Tree *t, int *out, size_t cap) {
    (void)t;
    (void)out;
    (void)cap;
    return 0;
}

size_t tree_levelorder(const Tree *t, int *out, size_t cap) {
    (void)t;
    (void)out;
    (void)cap;
    return 0;
}

size_t tree_sort(const int *in, size_t n, int *out, size_t cap) {
    (void)in;
    (void)n;
    (void)out;
    (void)cap;
    return 0;
}

size_t tree_range(const Tree *t, int lo, int hi, int *out, size_t cap) {
    (void)t;
    (void)lo;
    (void)hi;
    (void)out;
    (void)cap;
    return 0;
}
