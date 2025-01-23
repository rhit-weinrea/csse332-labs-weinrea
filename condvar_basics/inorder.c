#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;  
int cur = 1;

void *thread(void *arg)
{

   printf("%d wants to enter the critical section\n", *num);
  int *num = (int *)arg;
  pthread_mutex_lock(&m);

  while(*num != cur){
    pthread_cond_wait(&c, &m);
  }
  cur++;

  printf("%d is finished with the critical section\n", *num);

  pthread_cond_signal(&c);
  pthread_mutex_unlock(&m);

  return NULL;
}
int
main(int argc, char **argv)
{
  pthread_t threads[4];
  int i;
  int nums[] = {2, 1, 4, 3};

  for(i = 0; i < 4; ++i) {
    pthread_create(&threads[i], NULL, thread, &nums[i]);

    if(i == 2)
      sleep(3);
  }

  for(i = 0; i < 4; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished\n");

  return 0;
}
