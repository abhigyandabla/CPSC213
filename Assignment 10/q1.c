#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"

uthread_t t0, t1, t2;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void* p0(void* v) {
  randomStall();
  printf("zero\n");
  return NULL;
}

void* p1(void* v) {
  randomStall();
  printf("one\n");
  return NULL;
}

void* p2(void* v) {
  randomStall();
  printf("two\n");
  return NULL;
  }

int main(int arg, char** arv) {
  uthread_init(4);
  t0 = uthread_create(p0, NULL);
  uthread_join(t0, NULL);
  t1 = uthread_create(p1, NULL);
  uthread_join(t1, NULL);
  t2 = uthread_create(p2, NULL);
  uthread_join(t2, NULL);
  randomStall();
  printf("three\n");
  printf("------\n");
}