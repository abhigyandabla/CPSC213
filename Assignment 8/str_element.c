#include "str_element.h"
#include "refcount.h"
#include <stdio.h>
#include <string.h>

struct str_element {
    struct str_element_class *class;
    char* word;
};

void str_element_print(struct element *e);
int str_compare(struct element *e1, struct element *e2);
int is_str_element(struct element *e);
char* str_element_get_value(struct str_element *e);
struct str_element *str_element_new(char* s);
void str_finalizer(void* e);

struct str_element_class
{
    void (*print) (struct element *);
    int (*compare) (struct element*, struct element*);
    int (*is_str_element) (struct element *);
    char* (*str_element_get_value) (struct str_element *);
    struct str_element* (*str_element_new) (char*);
    void (*str_finalizer) (void* e);
};

struct str_element_class str_element_class = {str_element_print, str_compare};
void str_element_print(struct element *e) {
    struct str_element* s = (struct str_element*) e;
    printf("%s", s->word);
}

int str_compare(struct element *e1, struct element *e2) {
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
    if (int_element_get_value(i1) == int_element_get_value(i2))
    {
        return 0;
    } else if (int_element_get_value(i1) > int_element_get_value(i2))
    {
        return 1;
    } else {
        return -1;
    }
}

int is_str_element(struct element *e) {
    struct str_element* s = (struct str_element*) e;
    if (s->class == &str_element_class) {
        return 1;
    }
    return 0;
}

char* str_element_get_value(struct str_element *e) {
    struct str_element* strElement = (struct str_element*) e;
    return strElement->word;
}

void str_element_finalizer(void* e){
    struct str_element *s = (struct str_element*) e;
    free(s->word);
}

struct str_element* str_element_new(char* word) {
    struct str_element* s = rc_malloc(sizeof(struct str_element), str_element_finalizer);
    s->word = malloc(sizeof(char) * (strlen(word) + 1));
    s->class = &str_element_class;
    strcpy(s->word, word);
    return s;
}