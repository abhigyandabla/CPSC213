#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create(agent->mutex);
  agent->match   = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke   = uthread_cond_create(agent->mutex);
  return agent;
}

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

struct SmokerPool {
  struct Agent* agent;
  int match;
  int paper;
  int tobacco;
};

struct Smoker {
  struct SmokerPool* pool;
  int type;
};

struct Smoker* makeSmoker(int type, struct SmokerPool* pool){
  struct Smoker* s = malloc(sizeof(struct Smoker));
  s->pool = pool;
  s->type = type;
  return s;
}

struct SmokerPool* makeSmokerPool(struct Agent* agent){
  struct SmokerPool* smokerpool = malloc(sizeof (struct SmokerPool));
  smokerpool->tobacco = 0;
  smokerpool->paper = 0;
  smokerpool->match = 0;
  smokerpool->agent = agent;
  return smokerpool;
}


void smokerMATCH(struct Agent* dealer, struct SmokerPool* sesh, struct Smoker* s) {
  uthread_cond_wait(dealer->match);
  if (sesh->paper <= 0 || sesh->tobacco <= 0) {
    sesh->match++;
  } else {
    sesh->paper--;
    sesh->tobacco--;
    smoke_count[s->type]++;
    uthread_cond_signal(dealer->smoke);
  }
}

void smokerPAPER(struct Agent* dealer, struct SmokerPool* sesh, struct Smoker* s) {
  uthread_cond_wait(dealer->paper);
  if (sesh->match <= 0 || sesh->tobacco <= 0) {
    sesh->paper++;
  } else {
    sesh->match--;
    sesh->tobacco--;
    smoke_count[s->type]++;
    uthread_cond_signal(dealer->smoke);
  }
}

void smokerTOBACCO(struct Agent* dealer, struct SmokerPool* sesh, struct Smoker* s) {
  uthread_cond_wait(dealer->tobacco);
  if (sesh->match <= 0 || sesh->paper <= 0) {
    sesh->tobacco++;
  } else {
    sesh->match--;
    sesh->paper--;
    smoke_count[s->type]++;
    uthread_cond_signal(dealer->smoke);
  }
}

void signalDealer(struct Agent* dealer, struct SmokerPool* sesh) {
  if (sesh->paper > 0 && sesh->match > 0) {
      uthread_cond_signal(dealer->tobacco);
    } else if (sesh->paper > 0 && sesh->tobacco > 0) {
      uthread_cond_signal(dealer->match);
    } else if (sesh->tobacco > 0 && sesh->match > 0) {
      uthread_cond_signal(dealer->paper);
    }
}

void* smoker (void* blazer) {
  struct Smoker* s = blazer;
  struct SmokerPool* sesh = s->pool;
  struct Agent* dealer = sesh->agent;

  uthread_mutex_lock(dealer->mutex);

  while ("CPSC 213 gives me anxety" == "CPSC 213 gives me anxety") {

    if (s->type == MATCH) {
      smokerMATCH(dealer, sesh, s);
    } else if (s->type == PAPER){
      smokerPAPER(dealer, sesh, s);
    } else if (s->type == TOBACCO){
      smokerTOBACCO(dealer, sesh, s);
    }

    signalDealer(dealer, sesh);
    
  }
  uthread_mutex_unlock(dealer->mutex);
}


/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can modify it if you like, but be sure that all it does
 * is choose 2 random resources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  srandom(time(NULL));
  
  uthread_mutex_lock (a->mutex);
  // Wait until all other threads are waiting for a signal
  while (num_active_threads < 3)
    uthread_cond_wait (a->smoke);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 6;
    switch(r) {
    case 0:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    case 1:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 2:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 3:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 4:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 5:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    }
    VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
    uthread_cond_wait (a->smoke);
  }
  
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  
  struct Agent* a = createAgent();
  uthread_t agent_thread;
  struct SmokerPool* smokerpool = makeSmokerPool(a);

  uthread_init(5);
  
  uthread_t match = uthread_create(smoker, makeSmoker(MATCH, smokerpool));
  num_active_threads++;
  uthread_t paper = uthread_create(smoker, makeSmoker(PAPER, smokerpool));
  num_active_threads++;
  uthread_t tobacco = uthread_create(smoker, makeSmoker(TOBACCO, smokerpool));
  num_active_threads++;

  agent_thread = uthread_create(agent, a);
  uthread_join(agent_thread, NULL);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);

  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);

  return 0;
}
