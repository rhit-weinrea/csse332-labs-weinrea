#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 3
//
// Name: 


// Default buffer size for use with snprintf
#define BUFSIZE 128
#define NUM_CHILDREN 10

// setsighandler()
void setsighandler(int signum, void (*handler)(int))
{
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  sigaction(signum, &act, NULL);
}
volatile int time = 0;
void overtime(int signum)
{
time = 1;
}
int main(int argc, char **argv) {
  printf("starting\n"); 
  int pids[NUM_CHILDREN];
  int parent_pid = getpid();
  int status; 
  
  for(int i = 0; i  < NUM_CHILDREN; i++)
  {
	pid_t ppid = getppid();
	pid_t pid = fork();
	char parent[16];
	setsighandler(SIGALRM, overtime);
	snprintf(parent, sizeof(parent), "%d", ppid);
	if(pid == 0)
	{
		
		if((getpid() % 4 == 0))
		{
			exit(5);
		} else {
		 if (execlp("./bonus.bin", "./bonus.bin", parent, "Be brave and never give up!", (char *)NULL) == -1) {
			         perror("execlp failed");
				     }
		}
		
	}
	else
	{
	if((pid % 4) == 0)
	{
	   printf("Child %d is bad, will not execute\n", pid);
	}
	pids[i] = pid;	
	}
  }


  for(int i = 0; i < NUM_CHILDREN; i++)
  {	
	time = 0;
	alarm(5);
	waitpid(pids[i], &status, 0);
	alarm(0);
	if(time)
	{
	  printf("Child %d with PID %d timed out\n", i, pids[i]);
	  continue;
	}
	if(WIFEXITED(status) == 0)
	{
	   printf("Child %d with PID %d crashed!\n", i, pids[i]);
	} else if (WIFEXITED(status) == 5){

	} else if (WIFEXITED(status) == 6) {
	printf("Child %d with PID %d timed out\n", i, pids[i]);

	} else {	
	printf("Child %d with PID %d finished with exit code %d\n", i, pids[i], WIFEXITED(status));
	}
	alarm(0);
  }

  printf("Parent %d finished...\n", getpid());
  exit(0);
}
 	
