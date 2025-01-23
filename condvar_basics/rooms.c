#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Constants in the code
#define FIRST_ROOM_CAPACITY 5
#define FIRST_ROOM_SERVICE_RATE 1

#define SECOND_ROOM_CAPACITY 2
#define SECOND_ROOM_SERVICE_RATE 2
#define WAITING_ROOM_CAPACITY 2

#define NUM_PEOPLE_ARRIVING 10

// capture the of customers that leave early
int num_left_customers = 0;
// TODO: Define other state of the world variables here

// TODO: Define your mutex locks and condition variables:


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t first_room_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t second_room_cond = PTHREAD_COND_INITIALIZER;

int num_in_first_room = 0;
int num_in_second_room = 0;
int num_in_waiting_room = 0;
int num_unhappy_customers = 0;

void *customer(void *arg) {
    long tid = (long)arg;

    printf("[Customer %ld] Just arrived at first room...\n", tid);

    pthread_mutex_lock(&lock);
    while (num_in_first_room >= FIRST_ROOM_CAPACITY) {
        pthread_cond_wait(&first_room_cond, &lock);
    }
    num_in_first_room++;
    pthread_mutex_unlock(&lock);

    printf("[Customer %ld] Entered first room...\n", tid);

    sleep(FIRST_ROOM_SERVICE_RATE);

    pthread_mutex_lock(&lock);
    num_in_first_room--;
    pthread_cond_signal(&first_room_cond);
    pthread_mutex_unlock(&lock);

    printf("[Customer %ld] Left first room...\n", tid);

    pthread_mutex_lock(&lock);
    while (num_in_second_room >= SECOND_ROOM_CAPACITY) {
        if (num_in_waiting_room < WAITING_ROOM_CAPACITY) {
            num_in_waiting_room++;
            printf("[Customer %ld] Joined the waiting room for second room...\n", tid);
            pthread_cond_wait(&second_room_cond, &lock);
            num_in_waiting_room--;
        } else {
            num_unhappy_customers++;
            pthread_mutex_unlock(&lock);
            printf("[Customer %ld] Left unhappy because waiting room is full...\n", tid);
            return NULL;
        }
    }
    num_in_second_room++;
    pthread_mutex_unlock(&lock);

    printf("[Customer %ld] Entered second room...\n", tid);

    sleep(SECOND_ROOM_SERVICE_RATE);

    pthread_mutex_lock(&lock);
    num_in_second_room--;
    pthread_cond_signal(&second_room_cond);
    pthread_mutex_unlock(&lock);

    printf("[Customer %ld] Left second room...\n", tid);

    return NULL;
}

int
main(int argc, char **argv)
{
  int i;
  long int tids[NUM_PEOPLE_ARRIVING];
  pthread_t threads[NUM_PEOPLE_ARRIVING];
  srand(time(0));

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, customer, (void*)tids[i]);
    if(!(i % 2))
      sleep(rand() % 2);
  }

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everything finished: %d customers left unhappy...\n",
         num_left_customers);
  exit(EXIT_SUCCESS);
}
