/**
 * @file sample2_attach.c
 * @brief Peek the event/signals sent to the process
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

int
main(int argc, char **argv)
{
  const char *const target = "./target/target2";
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

  /* debugger main events loop */
  do {
    /* block and wait for an event to be sent to the target process */
    pid = waitpid(-1, &status, 0); /* equivalents to wait(&status) */

    if( WIFEXITED(status) ) {
      printf("log: process %ld exit normally with exit status %d\n", (long) pid, WEXITSTATUS(status));
    }
    else if( WIFSIGNALED(status) ) {
      printf("log: process %ld received %s termination signal\n", (long) pid, strsignal(WTERMSIG(status)));
    }
    else if( WIFSTOPPED(status) ) {
      switch( WSTOPSIG(status) ) {
        case SIGSTOP:
          /* handle SIGSTOP */
          break;
        case SIGTRAP:
          /* handle SIGTRAP: target loading with exec* or breakpoints */
          if( just_loaded ) {
            printf("log: process %ld is just loaded into the debugger\n", (long) pid);
            just_loaded = 0;
          } else {
            printf("log: process %ld hit a breakpoint\n", (long) pid);
          }
          break;
        case SIGFPE:
          /* handle SIGFPE: arithmetic errors */
          printf("log: process %ld had an arithmetic error\n", (long) pid);
          break;
        case SIGSEGV:
          /* handle SIGSEGV: memory invalid access errors */
          printf("log: process %ld had a memory invalid access\n", (long) pid);
          break;
        case SIGILL:
          /* handle SIGILL: invalid instructions */
          printf("log: process %ld attempt to execute invalid instructions\n", (long) pid);
          break;
        /* more events and signals */
        default:
          printf("log: process %ld received %s\n", (long) pid, strsignal(WSTOPSIG(status)));
      }
      /* continue the process */
      ptrace(PTRACE_CONT, pid, NULL, NULL);
    }
    else if( WIFCONTINUED(status) ) {
      printf("log: process %ld continued\n", (long) pid);
    }

    /* stop debugging when the target exit or receives termination signal */
  } while( !WIFEXITED(status) && !WIFSIGNALED(status) );


  return 0;
}
