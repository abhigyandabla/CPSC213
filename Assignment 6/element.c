#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "refcount.h"


/* See the header file for the descriptions of each function. */
struct element {
  int num;
  char *value;
};

/** Create a new element, which holds the given number and string.
 *
 * Postcondition: the created element has its own copy of the string.
 */
struct element *element_new(int num, char *value) {
  struct element *e = rc_malloc(sizeof(*e));
  if(e == NULL) {
    /* out of memory? */
    return NULL;
  }

  e->num = num;
  e->value = rc_malloc(strlen(value) + 1);
  strcpy(e->value, value);
  
  return e;
}

/** Delete an element, freeing the memory associated with it. */
void element_delete(struct element *e) {
  rc_free_ref(e->value);
  rc_free_ref(e);
}

/** Get the stored number from an element.
 *
 */
int element_get_num(struct element *e) {
  return e->num;
}

/** Get the stored string from an element.
 *
 * Returns: a pointer to the string value, which is still owned by the element.
 * Do not free or modify the returned string; make a copy if you want to keep
 * it past the element's lifetime.
 */
char *element_get_value(struct element *e) {
  return e->value;
}


void elem_val_inc(struct element *e) {
  rc_keep_ref(e->value);
}
