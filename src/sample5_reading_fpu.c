/**
 * @file sample4_reading_registers.c
 * @brief Reading process registers
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

static void
dump_registers(pid_t pid);

int
main(int argc, char **argv)
{
  const char *const target = "./target/target4";
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
      printf("log: process %ld exit with status %d\n", (long) pid, WEXITSTATUS(status));
    }
    /* SIGTRAP indicates the loading of the target process or a breakpoint */
    else if( WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP ) {
      if( just_loaded ) {
        printf("log: process %ld has been loaded into the debugger (SIGTRAP)\n", (long) pid);
        just_loaded = 0;
      }
      else {
        printf("log: process %ld hit a breakpoint\n", (long) pid);
        dump_registers(pid);
      }
      /* continue the process */
      _check_if_not_fail("ptrace", ptrace(PTRACE_CONT, pid, NULL, NULL));
    }
  } while( !WIFEXITED(status) && !WIFSIGNALED(status) );

  return 0;
}

struct fpxregs_struct
{
  unsigned short int cwd;
  unsigned short int swd;
  unsigned short int twd;
  unsigned short int fop;
  long int fip;
  long int fcs;
  long int foo;
  long int fos;
  long int mxcsr;
  long int reserved;
  struct {
    long double st0 __attribute__((aligned(16))); /* long double 12/16 byte aligned on 16 byte */
    long double st1 __attribute__((aligned(16))); /* each, 2 or 0 bytes are padded. */
    long double st2 __attribute__((aligned(16)));
    long double st3 __attribute__((aligned(16)));
    long double st4 __attribute__((aligned(16)));
    long double st5 __attribute__((aligned(16)));
    long double st6 __attribute__((aligned(16)));
    long double st7 __attribute__((aligned(16)));
  };
  long int xmm_space[32];  /* 8*16 bytes for each XMM-reg = 128 bytes */
  long int padding[56];
};


static void
dump_registers(pid_t pid)
{
  struct fpxregs_struct fpregs;

  _check_if_not_fail("ptrace", ptrace(PTRACE_GETFPXREGS, pid, NULL, &fpregs));

  printf("\nFP registers values:\n\n");
  printf("st0 => %Lf\n", fpregs.st0);
  printf("st1 => %Lf\n", fpregs.st1);
  printf("st2 => %Lf\n", fpregs.st2);
  printf("st3 => %Lf\n", fpregs.st3);
  printf("st4 => %Lf\n", fpregs.st4);
  printf("st5 => %Lf\n", fpregs.st5);
  printf("st6 => %Lf\n", fpregs.st6);
  printf("st7 => %Lf\n", fpregs.st7);

  printf("\nst0 brefore => %Lf\n", fpregs.st0);
  fpregs.st0 = 11.11;
  _check_if_not_fail("ptrace", ptrace(PTRACE_SETFPXREGS, pid, NULL, &fpregs));
  _check_if_not_fail("ptrace", ptrace(PTRACE_GETFPXREGS, pid, NULL, &fpregs));
  printf("st0 after   => %Lf\n\n", fpregs.st0);
}
