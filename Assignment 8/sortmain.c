#include <stdio.h>
#include <stdlib.h>

#include "int_element.h"
#include "str_element.h"
#include "element.h"

/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}

int compare(void* e_one, void* e_two) {
  return (*(struct element**) e_one)->class->compare((*(struct element**) e_one), *(struct element**) e_two);
}

int main(int argc, char **argv) {
  int n = argc - 1;
  struct element** list = malloc(sizeof(struct element*) * n);
  int i = 0;
  while (i<n){
    struct element *e = parse_string(argv[i+1]);
    list[i] = e;
    i++;
  }
  struct element* e = malloc(sizeof(struct element));

  /* TODO: Sort elements with qsort */
  qsort((void**) list, n, sizeof(e), compare);
  free(e);
  
  printf("Sorted: ");
  /* TODO: Print elements, separated by a space */
  i = 0;
  while(i < n)
  {
    list[i]->class->print(list[i]);
    rc_free_ref(list[i]);
    printf(" ");
    i++;
  }
  printf("\n");
  free(list);
}