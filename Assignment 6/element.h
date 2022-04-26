/* This header file defines the element structure and functions to operate on it. */
#pragma once

/** Structure forward declaration. Without a full declaration of the structure,
   the structure implementation is "private" to other modules - they must only use the public API. */
struct element;

/** Create a new element, which holds the given number and string.
 *
 * Postcondition: the created element has its own copy of the string.
 */
struct element *element_new(int num, char *value);

/** Delete an element, freeing the memory associated with it. */
void element_delete(struct element *e);

/** Get the stored number from an element.
 *
 */
int element_get_num(struct element *e);

/** Get the stored string from an element.
 *
 * Returns: a pointer to the string value, which is still owned by the element.
 * Do not free or modify the returned string; make a copy if you want to keep
 * it past the element's lifetime.
 */
char *element_get_value(struct element *e);

void elem_val_inc(struct element *e);
