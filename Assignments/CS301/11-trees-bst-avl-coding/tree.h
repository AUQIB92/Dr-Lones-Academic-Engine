/*
 * tree.h -- the contract for Coding Assignment 11 (Trees, BSTs and AVL),
 * Week 11's institute practicals P8 (sort via BST) and P12 (BST and
 * traversals).
 *
 * This header is given to you; do NOT change it. Your job is to provide the
 * functions below in tree.c (start from starter_tree.c).
 *
 * The tree is a MULTISET of int keys: the same key may occur more than once.
 * Duplicate keys are placed in the right subtree of an equal key. Inorder
 * therefore prints equal keys together, in nondecreasing order.
 *
 * MEMORY. Every node is allocated by tree_insert (or by the internal build in
 * tree_sort) and released by tree_delete/tree_remove_all/tree_destroy. The
 * table owns nothing else: keys are plain ints, so there are no key copies.
 *
 * FAILURE. tree_create returns NULL only if allocation fails. tree_insert
 * returns -1 if allocation fails and 1 otherwise. Every query is NULL-safe:
 * a NULL tree has size 0, height -1, no keys, and search/count return 0.
 * A traversal with out == NULL or cap == 0 writes nothing and returns 0.
 *
 * The deck's C is at Slides/CS301/11-trees-bst-avl.tex: the node layout and
 * traversals (Section 2), BST search/insert/delete (Section 4), and the AVL
 * rules (Section 5, for the written comparison only -- this assignment
 * implements the plain BST).
 */

#ifndef TREE_H
#define TREE_H

#include <stddef.h>

typedef struct Tree Tree;

/* Make an empty tree. Returns NULL only on allocation failure. */
Tree *tree_create(void);

/* Release every node. NULL-safe. */
void tree_destroy(Tree *t);

/*
 * Insert one occurrence of key.
 *   - returns 1 on success, -1 on allocation failure (or t == NULL);
 *   - duplicates are allowed and are placed in the right subtree.
 */
int tree_insert(Tree *t, int key);

/*
 * Delete ONE occurrence of key.
 *   - returns 1 if an occurrence was removed, 0 if key was absent;
 *   - the two-child case uses the inorder successor (the smallest key in the
 *     right subtree) and frees the removed node.
 */
int tree_delete(Tree *t, int key);

/* Remove every occurrence of key; returns how many were removed. */
size_t tree_remove_all(Tree *t, int key);

/* 1 if key occurs at least once, else 0. */
int tree_search(const Tree *t, int key);

/* Number of occurrences of key. */
size_t tree_count(const Tree *t, int key);

/* Number of keys stored (with multiplicity); 0 for NULL. */
size_t tree_size(const Tree *t);

/* Height in edges: -1 for the empty tree, 0 for a single node. */
int tree_height(const Tree *t);

/* Smallest / largest key. Return 1 and store it in *out, or 0 if empty.
 * A NULL out pointer means "just tell me whether the tree is non-empty". */
int tree_min(const Tree *t, int *out);
int tree_max(const Tree *t, int *out);

/*
 * Traversals. Fill out[0..cap-1] with the first cap keys in the named order
 * and return how many keys were written (at most cap). The tree is not
 * modified. Inorder is nondecreasing; preorder is node-left-right; postorder
 * is left-right-node; level order is breadth-first, left to right.
 */
size_t tree_inorder(const Tree *t, int *out, size_t cap);
size_t tree_preorder(const Tree *t, int *out, size_t cap);
size_t tree_postorder(const Tree *t, int *out, size_t cap);
size_t tree_levelorder(const Tree *t, int *out, size_t cap);

/*
 * Copy the keys of in[0..n-1] into out in nondecreasing order, using a BST
 * (P8: sort via BST). Writes at most cap keys and returns the number written.
 * Duplicates are preserved. in and out may be the same array. If out is NULL
 * or cap is 0, nothing is written and 0 is returned.
 */
size_t tree_sort(const int *in, size_t n, int *out, size_t cap);

/*
 * Copy every key k with lo <= k <= hi into out, in nondecreasing order,
 * writing at most cap keys; return the number written. A NULL tree or a
 * lo > hi range writes nothing.
 */
size_t tree_range(const Tree *t, int lo, int hi, int *out, size_t cap);

#endif /* TREE_H */
