#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 5
//
// Name: 

// IMPORTANT: buffer size for use with ALL reads and writes
#define BUFSIZE 1000
int main(int argc, char **argv) {
  char buf[BUFSIZE];
  char r[BUFSIZE];
  char w[BUFSIZE];
  int fd[2];
  pipe(fd);
  pid_t pid = fork();
  int pip2[2];
  pipe(pip2);
  if(pid == 0)
  {	
	const char *msg = "goodbye world\0";
	char readpipe[BUFSIZE];
	snprintf(readpipe, sizeof(readpipe), "%d", fd[0]);
	write(fd[1], msg, 14);
	
	snprintf(buf, sizeof(buf), "%d", fd[1]);
	snprintf(r, sizeof(r), "%d", pip2[0]);
	snprintf(w, sizeof(w), "%d", pip2[1]);
	close(fd[1]);
	execlp("./buffalopipe.bin", "buffalopipe.bin", readpipe, buf, r, w, NULL);	
	close(pip2[1]);
	exit(1);

  } else {
	close(fd[1]);
	
	close(pip2[1]);
	int status;
	waitpid(pid, &status, 0);
	char buff[BUFSIZE];
	read(pip2[0], buff, BUFSIZE);
	printf("%s", buff);
	close(pip2[0]);
	if(WIFEXITED(status)) {
		printf("buffalosay exited with exit code %d\n", WEXITSTATUS(status));
	}

  }
  printf("Parent %d finished ....\n", getpid());
  exit(0);
}


