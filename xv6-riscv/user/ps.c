#include "kernel/types.h"
#include "kernel/ps.h"
#include "user/user.h"

#ifdef PS_SYSCALL_DONE
static const char *ststr(int state)
{
  switch(state) {
  case 0:
    return "UNUSED";
    break;
  case 1:
    return "USED";
    break;
  case 2:
    return "SLEEPING";
    break;
  case 3:
    return "RUNNABLE";
    break;
  case 4:
    return "RUNNING";
    break;
  case 5:
    return "ZOMBIE";
    break;
  default:
    return "UNKNOWN";
  }
}
#endif

int
main(int argc, char **argv)
{
#ifndef PS_SYSCALL_DONE
  printf("[ERROR] ps system call not yet implemented.\n");
  printf("\t If you are done and are still seeing this message, READ THE INSTRUCTIONS\n");
  exit(1);
#else
  struct psstat my_ps;
  int pid;

  if((argc > 1 && !strcmp(argv[1], "-h")) || argc > 2) {
    printf("[Usage] ps_test <PID>\n");
    printf("\t Print process information for process with pid = <PID>");
    exit(0);
  } else if(argc == 2) {
    pid = atoi(argv[1]);
  } else {
    pid = getpid();
  }

  printf("***** Looking for process information for pid = %d\n", pid);

  // make the system call and check for errors!
  if(ps(pid, &my_ps)) {
    printf("[ERROR] Problem running ps for pid = %d\n", pid);
    exit(1);
  }

  // let's print it out
  printf("Process %s (%d)\n", my_ps.name, my_ps.pid);
  printf("================================================\n");
  printf("Process id:    %d\n", my_ps.pid);
  printf("Process name:  %s\n", my_ps.name);
  printf("Process state: %s\n\n", ststr(my_ps.state));

  printf("Parent process id:    %d\n", my_ps.ppid);
  printf("Parent process name:  %s\n", my_ps.pname);
  printf("Parent process state: %s\n", ststr(my_ps.pstate));
  printf("================================================\n\n");

  exit(0);
#endif
}
