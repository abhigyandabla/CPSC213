#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define MAX_THINKING_TIME 25000
#define VERBOSE

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

typedef struct fork {
  uthread_mutex_t lock;
  uthread_cond_t forfree;
  long free;
} fork_t;

int num_phils, num_meals;    
fork_t *forks;

void deep_thoughts() {
  usleep(random() % MAX_THINKING_TIME);
}

void initfork(int i) {
  forks[i].lock    = uthread_mutex_create();
  forks[i].forfree = uthread_cond_create(forks[i].lock);
  forks[i].free    = 1;
}

long getfork(long i) {
  uthread_mutex_lock(forks[i].lock);
  while (!forks[i].free) {
    uthread_cond_wait(forks[i].forfree);
  } 
  forks[i].free = 0;
  uthread_mutex_unlock(forks[i].lock);  return 1;
}

void putfork(long i) {
  uthread_mutex_lock(forks[i].lock);
  uthread_cond_signal(forks[i].forfree);
  forks[i].free = 1;
  uthread_mutex_unlock(forks[i].lock);
}

int leftfork(long i) {
  return i;
}

int rightfork(long i) {
  return (i + 1) % num_phils;
}

void *phil_thread(void *arg) {
  uintptr_t id = (uintptr_t) arg;
  int meals = 0;
  
  while (meals < num_meals) {
    deep_thoughts();
    getfork(rightfork(id));
    VERBOSE_PRINT ("\nPhilosopher %d gets right fork %ld.", id, rightfork(id));

    deep_thoughts();
    getfork(leftfork(id));
    VERBOSE_PRINT ("\nPhilosopher %d gets left fork %ld.", id, leftfork(id));

    deep_thoughts();
    VERBOSE_PRINT ("\nPhilosopher %d eats.", id);

    deep_thoughts();
    putfork(rightfork(id));
    VERBOSE_PRINT ("\nPhilosopher %d puts down right fork %ld.", id, rightfork(id));

    deep_thoughts();
    putfork(leftfork(id));
    VERBOSE_PRINT ("\nPhilosopher %d puts down left fork %ld.", id, leftfork(id));

    meals++;
  }
  
  return 0;
}

int main(int argc, char **argv) {

  uthread_t *p;
  uintptr_t i;
  
  if (argc != 3) {
    fprintf(stderr, "Usage: %s num_philosophers num_meals\n", argv[0]);
    return 1;
  }

  num_phils = strtol(argv[1], 0, 0);
  num_meals = strtol(argv[2], 0, 0);
  
  forks = malloc(num_phils * sizeof(fork_t));
  p = malloc(num_phils * sizeof(uthread_t));

  uthread_init(num_phils);
  
  srandom(time(0));
  for (i = 0; i < num_phils; ++i) {
    initfork(i);
  }

  i = 0;
  while (i < num_phils) {
    p[i] = uthread_create(phil_thread, (void*)i);
    i++;
  }
   int j = 0;
  while (j < num_phils) {
    uthread_join(p[j], 0);
    j++;
  }
  
  return 0;
}
