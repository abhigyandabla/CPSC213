#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

void strtoint(element_t* o, element_t i) {
	long* out = (long*) o;
	char* e;
	*out = strtol((char*) i, &e, 10);

	if (e == (char*) i) {
		*out = -1;
    }
}

void null_list_maker(element_t *addy, element_t string, element_t number) {
    char** addy1 = (char**) addy;
    
    if ((long) number != -1) {
        *addy1 = NULL;
    } else {
        *addy1 = (char*) string;
    }
}

int is_num_positive(element_t num) {
    return num >= 0;
}

int is_string_valid(element_t str) {
    return str != NULL;
}

void truncate(element_t *addy, element_t string, element_t length) {
    char* new_string = (char*) string;
    char** addy1 = (char**) addy;
    if (strlen(new_string) > (long) length) {
        new_string[(long) length] = 0;
    }
    *addy1 = new_string;
}

void print_nl(element_t str) {
    char* str1 =  (char *) str;
    printf("%s\n", str);
}

void print_space(element_t str) {
    char* str1 =  (char *) str;
    printf("%s ", str);
}

void max(element_t *addy, element_t num1, element_t num2) {
    long* addy1 = (long*) addy;

    if((long) num2 < (long) num1) {
        *addy1 = (long) num1;
    } else {
        *addy1 = (long) num2;
    }
}

int main (int argc, char* argv[]){

    struct list* input_list = list_create();
    list_append_array(input_list, (void*) (argv + 1), argc - 1);

    struct list* int_list = list_create();
	list_map1(strtoint, int_list, input_list);

    struct list* null_list = list_create();
    list_map2(null_list_maker, null_list, input_list, int_list); 

    struct list* positive_list = list_create();
    list_filter(is_num_positive, positive_list, int_list);

    struct list* only_string_list = list_create();
    list_filter(is_string_valid, only_string_list, null_list);

    struct list* trunced_list = list_create();
    list_map2(truncate, trunced_list, only_string_list, positive_list);

    list_foreach(print_nl, trunced_list);

    list_foreach(print_space, trunced_list);
    printf("\n");

    element_t max_num = 0;
    list_foldl(max, &max_num, int_list);
    printf("%ld", (long) max_num);
    printf("\n");

    list_destroy(input_list);
    list_destroy(int_list);
    list_destroy(null_list);
    list_destroy(positive_list);
    list_destroy(only_string_list);
    list_destroy(trunced_list);  
}
