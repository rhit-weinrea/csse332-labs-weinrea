/* Copyright 2021 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**
  Imagine a group of friends are getting together to play music, but
  they are arriving at different times.  Arriving can happen at any
  time (e.g. when some other friends are playing).

  There are 3 different kinds of friends - drummers, singers, and
  guitarists.  It takes one of each kind to make a band, plus only
  1 band can be playing at once.  Once those conditions are met, the
  players can start playing and stop playing in any order.  However,
  all 3 players must stop playing before a new set of 3 can start
  playing.

  Example output:

  drummer arrived
  drummer arrived
  guitarist arrived
  guitarist arrived
  singer arrived
  drummer playing
  guitarist playing
  singer playing
  singer arrived
  singer arrived
  drummer arrived
  guitarist arrived
  drummer finished playing
  guitarist finished playing
  singer finished playing
  singer playing
  guitarist playing
  drummer playing
  singer finished playing
  guitarist finished playing
  drummer finished playing
  guitarist playing
  drummer playing
  singer playing
  guitarist finished playing
  drummer finished playing
  singer finished playing
  Everything finished.


 **/

int DRUM = 0;
int SING = 1;
int GUIT = 2;

char* names[] = {"drummer", "singer", "guitarist"};



// because the code is similar, we'll just have one kind of thread
// and we'll pass its kind as a parameter

int drummers = 0;
int singers = 0;
int guitarists = 0;
int ready_to_play = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_form_band = PTHREAD_COND_INITIALIZER;
pthread_cond_t band_done = PTHREAD_COND_INITIALIZER;

// Ensures only one band is playing at a time
int band_playing = 0;

char* names[] = {"drummer", "singer", "guitarist"};

// Thread function
void* friend(void* kind_ptr) {
  int kind = *((int*)kind_ptr);
  
  pthread_mutex_lock(&mutex);
  printf("%s arrived\n", names[kind]);

  // Increment the count for the arriving friend type
  if (kind == 0) drummers++;
  else if (kind == 1) singers++;
  else if (kind == 2) guitarists++;

  // Check if a band can be formed
  while (drummers < 1 || singers < 1 || guitarists < 1 || band_playing) {
    pthread_cond_wait(&can_form_band, &mutex);
  }

  // Form a band: decrement counts and mark the band as playing
  drummers--;
  singers--;
  guitarists--;
  band_playing = 1;
  ready_to_play = 3;  // Number of members still playing

  pthread_mutex_unlock(&mutex);

  // Simulate playing
  printf("%s playing\n", names[kind]);
  sleep(1);
  printf("%s finished playing\n", names[kind]);

  pthread_mutex_lock(&mutex);
  ready_to_play--;

  // If all members of the current band are done, signal the next band
  if (ready_to_play == 0) {
    band_playing = 0;
    pthread_cond_broadcast(&can_form_band);
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

pthread_t friends[100];
int friend_count = 0;

void create_friend(int* kind) {
  pthread_create(&friends[friend_count], NULL, friend, kind);
  friend_count++;
}

int main(int argc, char **argv) {

  create_friend(&DRUM);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&GUIT);
  sleep(1);
  create_friend(&SING);
  create_friend(&SING);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&SING);

  // all threads must be created by this point
  // note if you didn't create an equal number of each, we'll be stuck forever
  for (int i = 0; i < friend_count; i++) {
    pthread_join(friends[i], NULL);
  }

  printf("Everything finished.\n");

}
