#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "uthread.h"
#include "uthread_sem.h"
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
  int num_cars;                 // number of cars
  enum Direction dir;           // direction
  int wait_num[2];              // wait number for EAST or WEST
  uthread_sem_t allowEntry[2];  // allow entry for EAST or WEST
  uthread_sem_t lock;           // lock
} Street;

struct Street* street;         // global variable for street

void initializeStreet(void) {
  street = malloc(sizeof(struct Street));             // allocate memory for street
  street->num_cars = 0;                               // initialize 0 cars on new street
  street->dir = 0;                                    // set street direction to east
  street->wait_num[EAST] = 0;                         // wait_num for EAST
  street->wait_num[WEST] = 0;                         // wait_num for WEST
  street->allowEntry[EAST] = uthread_sem_create(0);   // creating semaphore for EAST
  street->allowEntry[WEST] = uthread_sem_create(0);   // creating semaphore for WEST
  street->lock = uthread_sem_create(1);               // creating lock
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t   waitingHistogramLock;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_sem_wait (waitingHistogramLock);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal (waitingHistogramLock);
}


void enterStreet(enum Direction g) {
  uthread_sem_wait(street->lock);

  int isEmpty = !street->num_cars;
  int isFull = street->num_cars >= MAX_OCCUPANCY;
  int rightDir = street->dir == g;
  
  if (isEmpty || !isFull && rightDir) {
    street->num_cars++;
    entryTicker++;
    occupancyHistogram[g][street->num_cars]++;
  } else {
    street->wait_num[g]++;
  }
  uthread_sem_signal(street->lock);

  if (!isEmpty && !(!isFull && rightDir)) {
    uthread_sem_wait(street->allowEntry[g]);
  }
}


void leaveStreet(void) {
  uthread_sem_wait(street->lock);                    

  street->num_cars--;                                 
  if (street->wait_num[oppositeEnd[street->dir]] > 0) {
    if (!street->num_cars) {
      street->dir = oppositeEnd[street->dir];
      int i = 0;
      while (i < street->wait_num[street->dir] && i < MAX_OCCUPANCY) {
        uthread_sem_signal(street->allowEntry[street->dir]);
        street->wait_num[street->dir]--;
        street->num_cars++;
        entryTicker++;
        occupancyHistogram[street->dir][street->num_cars]++;
        i++;
      }
    }
  } else if (street->wait_num[street->dir] > 0) {
    uthread_sem_signal(street->allowEntry[street->dir]);
    street->wait_num[street->dir]--;
    street->num_cars++;
    entryTicker++;
    occupancyHistogram[street->dir][street->num_cars]++;
  }
  
  uthread_sem_signal(street->lock);                   
}


void* traffic_thread(enum Direction in) {
  int a = 0;                                
  while (a < NUM_ITERATIONS) {
    int wait = entryTicker;                // initial entryTicker
    enterStreet(in);                       // enter street with direction
    recordWaitingTime(entryTicker - wait); // recording wait time
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

  waitingHistogramLock = uthread_sem_create(1);
  
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