#include <stdio.h>
#include <stdlib.h>
#include <string.h>


///////
// This code implements a very simple version of malloc.
// It is correct and can be ignored while answering this quesiton.
// We are using it to simplify the debugging process.
char my_heap[4096];
int my_heap_cur = 0;
void* my_malloc(int nbytes) {
  void* result = &my_heap[my_heap_cur];
  my_heap_cur += nbytes;
  return result;
}
// The bug is below this line.
/////////

char *names[10];
int num_names = 0;

void add_name(char *name) {
  // Allocate space for a copy of the name
  names[num_names] = my_malloc(sizeof(name));
  // Copy name to new area in memory
  names[num_names] = name;
  // Add one to num_names
  num_names++;
}

void print_names(void) {
  for (int i = 0; i < num_names; i++) {
    printf("Name %d: %s\n", i + 1, names[i]);
  }
}

int main(void) {
  add_name("John Alexander Macdonald");
  add_name("Alexander Mackenzie");
  add_name("John Joseph Caldwell Abbott");
  add_name("John Sparrow David Thompson");
  add_name("Mackenzie Bowell");
  add_name("Charles Tupper");
  add_name("Henri Charles Wilfrid Laurier");
  add_name("Robert Laird Borden");
  add_name("Arthur Meighen");
  add_name("William Lyon Mackenzie King");
  
  print_names();
  return 0;
}
