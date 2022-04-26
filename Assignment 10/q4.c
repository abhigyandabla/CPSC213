#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

uthread_mutex_t mutex;
uthread_cond_t cond_1, cond_2;
uthread_cond_t cond;
int zero_done, one_done;
uthread_t t0, t1, t2;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void* p0(void* v) {
  uthread_mutex_lock(mutex);
  randomStall();
  printf("zero\n");
  zero_done = 1;
  uthread_mutex_unlock(mutex);
  return NULL;
}

void* p1(void* v) {
  uthread_mutex_lock(mutex);
  randomStall();
  if (!zero_done)
    uthread_cond_wait(cond);
  printf("one\n");
  one_done = 1;
  uthread_cond_signal(cond);
  uthread_mutex_unlock(mutex);
  return NULL;
}

void* p2(void* v) {
  uthread_mutex_lock(mutex);
  randomStall();
  if (!one_done)
    uthread_cond_wait(cond);
  printf("two\n");
  uthread_mutex_unlock(mutex);
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  zero_done = 0;
  one_done = 0;
  mutex = uthread_mutex_create();
  cond = uthread_cond_create(mutex);
  t0 = uthread_create(p0, NULL);
  t1 = uthread_create(p1, NULL);
  t2 = uthread_create(p2, NULL);
  randomStall();
  uthread_join (t0, NULL);
  uthread_join (t1, NULL);
  uthread_join (t2, NULL);
  printf("three\n");
  printf("------\n");
}
