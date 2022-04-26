#include "int_element.h"
#include "refcount.h"
#include <stdio.h>

struct int_element {
    struct int_element_class *class;
    int value;
};

void int_element_print(struct element *e);
int int_compare(struct element *e1, struct element *e2);
int is_int_element(struct element *e);
int int_element_get_value(struct int_element *e);
struct int_element *int_element_new(int value);

struct int_element_class
{
    void (*print) (struct element *);
    int (*compare) (struct element*, struct element*);
    int (*is_int_element) (struct element *);
    int (*int_element_get_value) (struct int_element *);
    struct int_element* (*int_element_new) (int);
};

struct int_element_class int_element_class = {int_element_print, int_compare};

void int_element_print(struct element* e) {
    struct int_element* i = (struct int_element*) e;
    printf("%d", i->value);
}

int int_compare(struct element* e1, struct element* e2) {
    struct int_element* i1 = (struct int_element*) e1;
    struct int_element* i2 = (struct int_element*) e2;
    if (!is_int_element(i1) && !is_int_element(i2)) {
        struct str_element* s1 = i1;
        struct str_element* s2= i2;
        return strcmp(str_element_get_value(s1), str_element_get_value(s2)); 
    }
     if (!is_int_element(i1)){
        return -1;
    }
     if (!is_int_element(i2)){
        return 1;
    }
    if (i1->value == i2->value)
    {
        return 0;
    } else if (i1->value > i2->value)
    {
        return 1;
    } else {
        return -1;
    }
}

int is_int_element(struct element *e) {
    struct int_element* i = (struct int_element*) e;
    if (i->class == &int_element_class) {
        return 1;
    }
    else{
    return 0;
    }
}

int int_element_get_value(struct int_element * e) {
    struct int_element* intElement = (struct int_element*) e;
    return intElement->value;
}

struct int_element* int_element_new(int value) {
    struct int_element* i = rc_malloc(sizeof(struct int_element), NULL);
    i->class = &int_element_class;
    i->value = value;
    return i;
}