/* Copyright 2016 Rose-Hulman
   But based on idea from http://cnds.eecs.jacobs-university.de/courses/caoslab-2007/
   */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
void setsighandler(int signum, void (*handler)(int)) {
	  struct sigaction act;
	  
	    act.sa_handler = handler;
	      sigemptyset(&act.sa_mask);
	        act.sa_flags = SA_RESTART;
		  sigaction(signum, &act, NULL);
}

void handle_sigchild(int ignored)
{
	int pid;
	int status;
	while((pid = waitpid(-1, &status, 0)) > 0)
	{
	  printf("Background command finished\n");	


	}
	wait(0);
}

int main() {
  setsighandler(SIGCHLD, handle_sigchild);
  char command[82];
  char *parsed_command[2];
  //takes at most two input arguments
  // infinite loop but ^C quits
  while (1) {
    printf("SHELL%% ");
    int bg = 0;
    fgets(command, 82, stdin);
    command[strlen(command) - 1] = '\0';//remove the \n
    int len_1;
    for(len_1 = 0;command[len_1] != '\0';len_1++){
      if(command[len_1] == ' ')
        break;
    }
    parsed_command[0] = command;
    if(len_1 == strlen(command)){
      printf("Command is '%s' with no arguments\n", parsed_command[0]);
      parsed_command[1] = NULL;
      char commandNum = parsed_command[0][0];
      char *ptr = command;
      //my stuff
      
      if(strncmp(command, "BG", 2) == 0)
      {
	bg = 1;
	parsed_command[0] += 2;
      }
      int count = 1;
      if(isdigit(command[0]))
      {
	printf("Will generate '%c' commands '%s'\n", command[0], parsed_command[0]);
	count = strtol(ptr, &ptr, 10);
	printf("num '%d'\n", count);
	parsed_command[0] += 1;
      }
      for(int i = 0; i < count; i++)
      {
	pid_t waitpids;
      	int status = -1;
      	pid_t pid;
      	pid = fork();
	int stat = -1;
	if(pid == 0)
	{
		
		if(bg)
		{
		waitpids = fork();
		
		if(waitpids == 0)
		{
		execlp(parsed_command[0], parsed_command[0], NULL);
		exit(5);	
		} else {
							
			//wait(&status);
			//printf("Background Command Finished\n");
			//exit(5);	
		}
		} else {
			execlp(parsed_command[0], parsed_command[0], NULL);
		}
	}
        else

	{
	  if(!bg)
	  {
		wait(&status);
	  } 
	  
	}
       	       

      
      
      }

      
      // leave this here, do not change it
      if(!strcmp(parsed_command[0], "quit") ||
          !strcmp(parsed_command[0], "exit")) {
        exit(EXIT_SUCCESS);
      }
    }else{
      command[len_1] = '\0';
      parsed_command[1] = command + len_1 + 1;
      printf("Command is '%s' with argument '%s'\n", parsed_command[0], parsed_command[1]);
      
      //my stuff
      int status = -1;
      pid_t pid;
      pid = fork();
      if(pid == 0)
      {
	execl(parsed_command[0], parsed_command[0], parsed_command[1], NULL);
      }
      else
      {
        wait(&status);
      } 
      // leave this here, do not change it
      if(!strcmp(parsed_command[0], "quit") ||
          !strcmp(parsed_command[0], "exit")) {
        exit(EXIT_SUCCESS);
      }
    }

  }
}
