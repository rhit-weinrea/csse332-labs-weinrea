#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 1
//
// Name: 


// Default buffer size for use with snprintf
#define BUFSIZE 128
#define NUM_CHILDREN 10

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

	snprintf(parent, sizeof(parent), "%d", ppid);
	if(pid == 0)
	{
		 if (execlp("./pretty.bin", "./pretty.bin", parent, "Be brave and never give up!", (char *)NULL) == -1) {
			         perror("execlp failed");
				     }
		
	}
	else
	{
	pids[i] = pid;	
	}
  }

  for(int i = 0; i < NUM_CHILDREN; i++)
  {
	waitpid(pids[i], &status, 0);
	if(WIFEXITED(status) == 0)
	{
	   printf("Child %d with PID %d crashed!\n", i, pids[i]);
	}
	printf("Child %d with PID %d finished with exit code %d\n", i, pids[i], WIFEXITED(status));
	
  }

  printf("Parent %d finished...\n", getpid());
  exit(0);
}

