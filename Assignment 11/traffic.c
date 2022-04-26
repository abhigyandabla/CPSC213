#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"
#include <assert.h>
#define VERBOSE

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

#define MAX_OCCUPANCY  3
#define NUM_ITERATIONS 100
#define NUM_CARS       20

// These times determine the number of times yield is called when in
// the street, or when waiting before crossing again.
#define CROSSING_TIME             NUM_CARS
#define WAIT_TIME_BETWEEN_CROSSES NUM_CARS

/**
 * You might find these declarations useful.
 */
enum Direction {EAST = 0, WEST = 1};
const static enum Direction oppositeEnd [] = {WEST, EAST};

struct Street {
  uthread_mutex_t mx;     // mutex
  int num_cars;           // number of cars
  enum Direction dir;     // direction
  uthread_cond_t east;    // east condition
  uthread_cond_t west;    // west condition
} Street;

struct Street* street;    // global variable for street

void initializeStreet(void) {
  street = malloc(sizeof(struct Street));           // allocate memory for street
  street->mx = uthread_mutex_create();              // create mutex for street
  street->num_cars = 0;                             // initialize 0 cars on new street
  street->dir = 0;                                  // set street direction to east
  street->east = uthread_cond_create(street->mx);   // create east condition
  street->west = uthread_cond_create(street->mx);   // create west condition
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogramLock;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

void enterStreet (enum Direction g) {
  assert(street->num_cars <= MAX_OCCUPANCY);
  uthread_mutex_lock(street->mx);                                   // locking mutex
  if (!street->num_cars)                                            // if no car on street
    street->dir = g;                                                // set new direction
  int pre_loop_ticker = entryTicker;                                // entryTicker before loop
  while (street->num_cars >= MAX_OCCUPANCY || g != street->dir) {   // if more cars can occupancy
    if (g == EAST)                                                  // or wrong direction, wait
      uthread_cond_wait(street->east);                              // east wait if g == EAST
    if (g == WEST)
      uthread_cond_wait(street->west);                              // west wait if g == WEST
  }
  int post_loop_ticker = entryTicker - pre_loop_ticker;             // difference in ticker before and after loop
  entryTicker++;                                                    // increment entryTicker
  recordWaitingTime(post_loop_ticker);                              // record waiting time
  street->num_cars++;                                               // increment number of cars in street
  occupancyHistogram[g][street->num_cars]++;                        // increment number of cars in dir lane
  uthread_mutex_unlock(street->mx);                                 // unlocking mutex
  assert(street->num_cars <= MAX_OCCUPANCY);
}

void leaveStreet(void) {
  uthread_mutex_lock(street->mx);             // lock mutex
  street->num_cars--;                         // decrement number of cars
  if (!street->num_cars) {                    // if there are no cars
    street->dir = oppositeEnd[street->dir];   // switch direction
    if (street->dir == EAST)                  // if direction is EAST
      uthread_cond_broadcast(street->east);   // broadcast EAST
    if (street->dir == WEST)                  // if direction is WEST
      uthread_cond_broadcast(street->west);   // broadcast WEST
  }
  uthread_mutex_unlock(street->mx);           // unlock mutex
  assert(street->num_cars <= MAX_OCCUPANCY);
}

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogramLock);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogramLock);
}

void* traffic_thread(enum Direction in) {
  int a = 0;                                
  while (a < NUM_ITERATIONS) {
    enterStreet(in);                       // enter street with direction
    for (int i = 0; i < NUM_CARS; i++)
      uthread_yield();                     // yield for all cars
    leaveStreet();                         // leave street
    for (int j = 0; j < NUM_CARS; j++)
      uthread_yield();                     // yield for all cars
    a++;
  }
}

int main (int argc, char** argv) {

  uthread_init(8);

  waitingHistogramLock = uthread_mutex_create();

  initializeStreet();
  uthread_t pt [NUM_CARS];

  for (int i = 0; i < NUM_CARS; i++) {
    int rand_int = random() % 2;
    if (!rand_int)
      pt[i] = uthread_create(traffic_thread, EAST);
    else
      pt[i] = uthread_create(traffic_thread, WEST);
  }

  for (int i = 0; i < NUM_CARS; i++) {
    uthread_join(pt[i], 0);
  }
  printf("entryTicker: %d\n", entryTicker);
  
  printf ("Times with 1 car  going east: %d\n", occupancyHistogram [EAST] [1]);
  printf ("Times with 2 cars going east: %d\n", occupancyHistogram [EAST] [2]);
  printf ("Times with 3 cars going east: %d\n", occupancyHistogram [EAST] [3]);
  printf ("Times with 1 car  going west: %d\n", occupancyHistogram [WEST] [1]);
  printf ("Times with 2 cars going west: %d\n", occupancyHistogram [WEST] [2]);
  printf ("Times with 3 cars going west: %d\n", occupancyHistogram [WEST] [3]);
  
  printf ("Waiting Histogram\n");
  for (int i=0; i < WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Cars waited for           %4d car%s to enter: %4d time(s)\n",
	      i, i==1 ? " " : "s", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Cars waited for more than %4d cars to enter: %4d time(s)\n",
	    WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
