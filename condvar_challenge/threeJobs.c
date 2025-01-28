/* Copyright 2019 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


// number of carpenters
#define NUM_CARP 3
// number of painters
#define NUM_PAIN 3
// number of decorators
#define NUM_DECO 3

/**
  Imagine there is a shared memory space called house.

  There are 3 different kinds of operations on house: carpenters, painters, and
  decorators.  For any particular kind of operation, there can be an unlimited
  number of threads doing the same operation at once (e.g. unlimited carpenter
  threads etc.).  However, only one kind of operation can be done at a time (so
  even a single carpenter should block all painters and vice versa).

  Use mutex locks and condition variables to enforce this constraint.  You don't
  have to worry about starvation (e.g. that constantly arriving decorators might
  prevent carpenters from ever running) - though maybe it would be fun to
  consider how you would solve in that case.

  This is similar to the readers/writers problem BTW.
 **/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t carpenters_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t painters_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t decorators_cond = PTHREAD_COND_INITIALIZER;

int current_operation = 0;  // 0: none, 1: carpentry, 2: painting, 3: decorating

void* carpenter(void * ignored) {
  pthread_mutex_lock(&mutex);
  while (current_operation != 0 && current_operation != 1) {
    pthread_cond_wait(&carpenters_cond, &mutex);
  }
  
  current_operation = 1;
  printf("starting carpentry\n");
  sleep(1);
  printf("finished carpentry\n");

  current_operation = 0;
  pthread_cond_broadcast(&painters_cond);
  pthread_cond_broadcast(&decorators_cond);
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void* painter(void * ignored) {
  pthread_mutex_lock(&mutex);
  while (current_operation != 0 && current_operation != 2) {
    pthread_cond_wait(&painters_cond, &mutex);
  }

  current_operation = 2;
  printf("starting painting\n");
  sleep(1);
  printf("finished painting\n");

  current_operation = 0;
  pthread_cond_broadcast(&carpenters_cond);
  pthread_cond_broadcast(&decorators_cond);
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void* decorator(void * ignored) {
  pthread_mutex_lock(&mutex);
  while (current_operation != 0 && current_operation != 3) {
    pthread_cond_wait(&decorators_cond, &mutex);
  }

  current_operation = 3;
  printf("starting decorating\n");
  sleep(1);
  printf("finished decorating\n");

  current_operation = 0;
  pthread_cond_broadcast(&carpenters_cond);
  pthread_cond_broadcast(&painters_cond);
  pthread_mutex_unlock(&mutex);

  return NULL;
}


int main(int argc, char **argv) {
  pthread_t jobs[NUM_CARP + NUM_PAIN + NUM_DECO];
  for (int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    void* (*func) (void*) = NULL;
    if(i < NUM_CARP)
      func = carpenter;
    if(i >= NUM_CARP && i < NUM_CARP + NUM_PAIN)
      func = painter;
    if(i >= NUM_CARP + NUM_PAIN) {
      func = decorator;
    }
    pthread_create(&jobs[i], NULL, func, NULL);
  }

  for (int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    pthread_join(jobs[i], NULL);
  }

  printf("Everything finished.\n");

}
