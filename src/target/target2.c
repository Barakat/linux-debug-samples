/**
 * @file target1.c
 * @brief Testing target 2
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include <stdio.h>
#include <signal.h>

int
main(int argc, char **argv)
{
  /* mock signals */
  raise(SIGSTOP);
  raise(SIGFPE);
  raise(SIGTRAP);
  raise(SIGSEGV);
  raise(SIGILL);
  raise(SIGCONT);
  return 123;
}
