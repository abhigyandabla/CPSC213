#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_THREADS 3
uthread_t threads[NUM_THREADS];
uthread_mutex_t mutex;
uthread_cond_t cond;
int num_threads_reached;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void waitForAllOtherThreads() {
  num_threads_reached++;
  if (num_threads_reached < NUM_THREADS) {
    uthread_cond_wait(cond);
  }
  uthread_cond_signal(cond);
}

void* p(void* v) {
  randomStall();
  printf("a\n");
  uthread_mutex_lock(mutex);
  waitForAllOtherThreads();
  uthread_mutex_unlock(mutex);
  printf("b\n");
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mutex = uthread_mutex_create();
  cond = uthread_cond_create(mutex);
  num_threads_reached = 0;
  for (int i=0; i<NUM_THREADS; i++)
    threads[i] = uthread_create(p, NULL);
  for (int i=0; i<NUM_THREADS; i++)
    uthread_join (threads[i], NULL);
  printf("------\n");
}