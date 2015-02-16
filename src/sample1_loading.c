/**
 * @file sample1_loading.c
 * @brief Loading a target process into the debugger
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

int
main(int argc, char **argv)
{
  const char *const target = "./target/target1";
  char * const args[] = {NULL};
  pid_t pid;
  int status;
  int just_loaded;

  /* fork to create a child process */
  switch( fork() ) {
    case -1:
      perror("Error");
      return -1;
    case 0:
      /* child process, trace it and create a new process of the target */
      _check_if_not_fail("ptrace", ptrace(PTRACE_TRACEME, 0, NULL, NULL) );
      _check_if_not_fail("execvp", execvp(target, args) );
      _exit(0);
  }

  just_loaded = 1;
  /* wait for a signal from the process */
  do {
    pid = waitpid(-1, &status, 0);
    /* check if the process has exit */
    if( WIFEXITED(status) ) {
      printf("log: process %ld exit normally\n", (long) pid);
    }
    /* SIGTRAP indicates the loading of the target process or a breakpoint */
    else if( WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP ) {
      if( just_loaded ) {
        printf("log: process %ld has been loaded into the debugger (SIGTRAP)\n", (long) pid);
        just_loaded = 0;
      }
      else {
        printf("log: process %ld hit a breakpoint\n", (long) pid);
      }
      /* continue the process */
      _check_if_not_fail("ptrace", ptrace(PTRACE_CONT, pid, NULL, NULL));
    }
  } while( !WIFEXITED(status) && !WIFSIGNALED(status) );


  return 0;
}
