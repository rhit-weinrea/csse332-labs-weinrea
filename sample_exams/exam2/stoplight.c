
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>



#define YELLOW 0
#define EW 1
#define NS 2

int status;

int cars = 0;

pthread_cond_t light = PTHREAD_COND_INITIALIZER;
pthread_cond_t EW_cars = PTHREAD_COND_INITIALIZER;
pthread_cond_t NS_cars = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *north_south(void *arg)
{
  //wait if light is yellow or EW
  //signal light if last car in intersection
  printf("northsouth car nearing intersection\n");
  pthread_mutex_lock(&mutex);
  while(status == YELLOW || status == EW)
  {
    pthread_cond_wait(&NS_cars, &mutex);
  }
  printf("northsouth car entering intersection\n");
  cars++;
  pthread_mutex_unlock(&mutex);
  sleep(1);
  printf("northsouth car leaving intersection\n");
  cars--;
  pthread_mutex_lock(&mutex);
  if(status == YELLOW && cars == 0){
    pthread_cond_signal(&light);
  }
  pthread_mutex_unlock(&mutex);
}


void *east_west(void *arg)
{

//wait if light is yellow or NS
//signal light if last car in intersection

  printf("eastwest car nearing intersection\n");
   pthread_mutex_lock(&mutex);
  while(status == YELLOW || status == NS)
  {
    pthread_cond_wait(&EW_cars, &mutex);
  }
  printf("eastwest car entering intersection\n");
  cars++;
  pthread_mutex_unlock(&mutex);
  sleep(1);
  printf("eastwest car leaving intersection\n");
   cars--;
  pthread_mutex_lock(&mutex);
  if(status == YELLOW && cars == 0){
    pthread_cond_signal(&light);
  }
  pthread_mutex_unlock(&mutex);
}


void *stoplight(void *arg)
{

  while(1) {

    //Light
    //wait if yellow and cars in intersection
    //signal NS cars when changing to light NS
    //signal EW cars when changing to light EW

    sleep(1);

    
    printf("yellow\n");
    status = YELLOW;
    pthread_mutex_lock(&mutex);
    while(cars > 0 && status == YELLOW){
      pthread_cond_wait(&light, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    // we need to wait for the intersection to clear
    printf("green in eastwest direction\n");
    status = EW;
    pthread_cond_signal(&EW_cars);
    
    sleep(1);
    printf("yellow\n");
    status = YELLOW;
    
    pthread_mutex_lock(&mutex);
    while(cars > 0 && status == YELLOW){
      pthread_cond_wait(&light, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    // we need to wait for the intersection to clear
    printf("green in northsouth direction\n");
    pthread_cond_signal(&NS_cars);
    status = NS;

  }

}

int main()
{

  pthread_t tid[100];
  int i = 0;

  // make sure we display the initial stoplight state before cars start
  printf("green in northsouth direction\n");
  status = NS;
  pthread_create(&tid[i++],NULL,stoplight,NULL);


  pthread_create(&tid[i++],NULL,east_west,NULL);
  pthread_create(&tid[i++],NULL,north_south,NULL);
  pthread_create(&tid[i++],NULL,north_south,NULL);
  sleep(1);
  pthread_create(&tid[i++],NULL,east_west,NULL);
  pthread_create(&tid[i++],NULL,north_south,NULL);
  sleep(1);
  pthread_create(&tid[i++],NULL,east_west,NULL);
  pthread_create(&tid[i++],NULL,north_south,NULL);



  for(int j = 1; j < i; j++) {
    pthread_join(tid[j],NULL);
  }


  return 0;
}
