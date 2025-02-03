#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "prettyprint.h"

#define NUM_THREADS 10

const char *stack = "abcdefghijklmnopqrstuvwxyz123456789";
int stackptr = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
char pop()
{
  if(stackptr >= strlen(stack)) {
    fprintf(stderr,
            "Buffer overflow when reading the stack. Fix your code!\n");
    return 0;
  }

  return stack[stackptr++];
}

int is_empty()
{
  return (stackptr >= strlen(stack));
}

// TODO: Add any extra functions you need here
void worker_thread(int num)
{
    pthread_mutex_lock(&m);
   while(!is_empty()){
	char c = pop;
	pthread_mutex_unlock(&m);
        printf("Thread %d processing request %c\n", num, pop());
	sleep(1);
	pthread_mutex_lock(&m);
   }
   pthread_mutex_unlock(&m);
  

}
int main(int argc, char **argv)
{
  pthread_t tid[NUM_THREADS];
  /* TODO: YOUR IMPLEMENTATION GOES HERE */
  for(int i = 0; i < NUM_THREADS; i++)
  {
	 printf("Worker thread %d started.\n", i);

	pthread_create(&tid[i], NULL, worker_thread, i);
  }
 
  
  for(int i = 0; i < NUM_THREADS; i++)
  {
	pthread_join(tid[i], NULL);
  }
  /* DO NOT REMOVE THIS LINE */
  printf("****** Everything finished ******\n");
}
