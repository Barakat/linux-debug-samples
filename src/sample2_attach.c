/**
 * @file sample2_attach.c
 * @brief Attaching a running process to the debugger and detach it
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

int
main(int argc, char **argv)
{
  pid_t pid;
  int status;

  if( argc < 2 ) {
    printf("usage: %s [target PID]\n", argv[0]);
    return -1;
  }

  /* attached to the target process */
  _check_if_not_fail("ptrace", ptrace(PTRACE_ATTACH, atol(argv[1]), NULL, NULL));

  /* wait for a signal from the process */
  pid = waitpid(-1, &status, 0);
  /* SIGSTOP indicates that the process is attached */
  if( WIFSTOPPED(status) && WSTOPSIG(status) == SIGSTOP ) {
    printf("process %ld is attached to the debugger\n", (long) pid);
    _check_if_not_fail("ptrace", ptrace(PTRACE_DETACH, pid, NULL, NULL));
    printf("process %ld is deattch\n", (long) pid);
  }

  return 0;
}
