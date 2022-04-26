#include <stdio.h>
#include "element.h"
#include "list.h"
#include "tree.h"
#include "refcount.h"

/**
 * Your solution cannot depend in any way on the implementation of this function.
 * That is, your program must work with any arbitrary selection function.
 */
int includeElement() {
  static int count=0;
  return (++count) % 2;
}

int main(int argc, char **argv) {
  struct tree *t = tree_new();
  struct list *l = list_new();

  for (int i=0; i<argc-1; i++)
    list_add_element(l, element_new(i+1, argv[i+1]));

  for(struct list_node *n = list_get_head(l); n != NULL; n = list_node_get_next(n)) {
    if(includeElement())
      tree_insert_node(t, list_node_get_element(n));
  }
  printf ("Before deleting list:\n");
  tree_ordered_print(t);
  list_delete(l);
  printf ("After deleting list:\n");
  tree_ordered_print(t);
  tree_delete(t);
}
