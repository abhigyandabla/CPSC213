#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "refcount.h"
#include "element.h"

struct tree {
  struct tree_node *root;
};

struct tree_node {
  struct element *elem;
  struct tree_node *left;
  struct tree_node *right;
};

/**
 * Create a new, empty tree
 */
struct tree *tree_new() {
  struct tree *t = malloc(sizeof(*t));
  t->root = NULL;
  return t;
}

static void tree_delete_helper(struct tree_node *n) {
  if (n != NULL) {
    tree_delete_helper(n->left);
    tree_delete_helper(n->right);
    element_delete(n->elem);
    free(n);
  }
}

/**
 * Delete tree and all of its nodes
 */
void tree_delete(struct tree* t) {
  tree_delete_helper(t->root);
  free(t);
}

static struct tree_node *tree_insert_node_helper(struct tree_node **np, struct element *e) {
  if(*np != NULL) {
    if(strcmp(element_get_value(e), element_get_value((*np)->elem)) <= 0)
      return tree_insert_node_helper(&(*np)->left, e);
    else
      return tree_insert_node_helper(&(*np)->right, e);
  } else {
    *np = malloc(sizeof(**np));
    (*np)->elem = e;
    rc_keep_ref(e);
    elem_val_inc(e);
    (*np)->left = (*np)->right = NULL;
    return *np;
  }
}

/**
 * Insert a new list element into the tree
 */
struct tree_node *tree_insert_node(struct tree *t, struct element *e) {
  return tree_insert_node_helper(&t->root, e);
}

static void tree_ordered_print_helper(struct tree_node* n, int depth) {
  if(n != NULL) {
    /* %*s takes two arguments, a width and a string, and pads the string to the width.
       By printing an empty string, this prints a desired number of spaces (indentation).
     */
    printf("%*s%d: %s\n", depth, "", element_get_num(n->elem), element_get_value(n->elem));
    tree_ordered_print_helper(n->left, depth + 1);
    tree_ordered_print_helper(n->right, depth + 1);
  }
}

/**
 * Print each node in a depth-first traversal of tree.
 */
void tree_ordered_print(struct tree *t) {
  tree_ordered_print_helper(t->root, 0);
}
