#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 4
//
// Name: 


// IMPORTANT: buffer size for use with ALL reads and writes
#define BUFSIZE 1000

int main(int argc, char **argv) {
  char buf[BUFSIZE];
  int fd[2];
  pipe(fd);
  pid_t pid = fork();
  if(pid == 0)
  {	
	const char *msg = "secret handshake";
	char readpipe[BUFSIZE];
	snprintf(readpipe, sizeof(readpipe), "%d", fd[0]);
	write(fd[1], msg, 16);
	write(fd[1], "\0", 1);	
	snprintf(buf, sizeof(buf), "%d", fd[1]);
	close(fd[1]);
	execlp("./buffalosay.bin", "buffalosay.bin", readpipe, buf, NULL);	
	exit(1);

  } else {
	close(fd[1]);


	int status;
	waitpid(pid, &status, 0);
	if(WIFEXITED(status)) {
		printf("buffalosay exited with exit code %d\n", WEXITSTATUS(status));
	}

  }
  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

