#define MAX_PROC_NAME 16

struct psstat {
  int pid;                        // The process ID.
  int ppid;                       // The parent process ID.

  int state;                      // The current process state.
  int pstate;                     // The current parent process state.

  char name[MAX_PROC_NAME];       // The process name.
  char pname[MAX_PROC_NAME];      // The parent process name.
};
