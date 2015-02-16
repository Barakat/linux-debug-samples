/**
 * @file sample10_breakpoint.c
 * @brief Settings debugger breakpoints.
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"
#include "memory_and_registers.h"


struct breakpoint_list {
  void *addr;
  unsigned char org_byte;
};

struct breakpoint_list bplist;

const unsigned char int3 = 0xcc;

static void
breakpoints(pid_t pid);

int
main(int argc, char **argv)
{
  const char *const target = "./target/target8";
  char * const args[] = {NULL};
  pid_t pid;
  int status;
  int breakpoint_counter;

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

  breakpoint_counter = 0;

  /* wait for a signal from the process */
  do {
    pid = waitpid(-1, &status, 0);
    /* check if the process has exit */
    if( WIFEXITED(status) ) {
      printf("log: process %ld exit with status %d\n", (long) pid, WEXITSTATUS(status));
    }
    /* SIGTRAP indicates the loading of the target process or a breakpoint */
    else if( WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP ) {
      if( breakpoint_counter == 0 ) {
        printf("log: process %ld has been loaded into the debugger (SIGTRAP)\n", (long) pid);
        breakpoint_counter++;
      }
      else if( breakpoint_counter == 1 ) {
        printf("log: process %ld hit first breakpoint, setting breakpoint on puts() ...\n", (long) pid);
        breakpoints(pid);
        breakpoint_counter++;
      }
      else {
        /* we hit a breakpoint we created */
        struct regs_struct regs;
        _check_if_not_fail("ptrace", ptrace(PTRACE_GETREGS, pid, NULL, &regs));

        /* check if it's the breakpoint we set */
        if(regs.eip - 1 == (uint32_t) bplist.addr) {
          printf("log: soft-breakpoint has been reached!, removing 0xcc/int3 to continue the process\n");
          /* restore the original byte */
          _check_if_not_fail("write_memory", write_memory(pid, (long) bplist.addr, 1, &bplist.org_byte));
          /* reset the eip to that address */
          regs.eip = (uint32_t) bplist.addr;
          _check_if_not_fail("ptrace", ptrace(PTRACE_SETREGS, pid, NULL, &regs));
        }
      }
      /* continue the process */
      _check_if_not_fail("ptrace", ptrace(PTRACE_CONT, pid, NULL, NULL));
    }
  } while( !WIFEXITED(status) && !WIFSIGNALED(status) );

  return 0;
}


static void
breakpoints(pid_t pid)
{
  struct regs_struct regs;

  /* get the function address, you can also resolve it */
  _check_if_not_fail("ptrace", ptrace(PTRACE_GETREGS, pid, NULL, &regs));
  printf("eax (puts) => 0x%08"PRIx32"\n", regs.eax);

  /* store the address */
  bplist.addr = (void *) regs.eax;

  /* store the byte at that address and replace it with 0xcc */
  _check_if_not_fail("read_memory", read_memory(pid, (long) bplist.addr, 1, &bplist.org_byte));
  _check_if_not_fail("write_memory", write_memory(pid, (long) bplist.addr, 1, &int3));
}
