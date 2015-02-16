/**
 * @file sample7_memory.c
 * @brief Reading and writting to process memory
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"
#include "memory_and_registers.h"

static void
dump_memory(pid_t pid);

int
main(int argc, char **argv)
{
  const char *const target = "./target/target6";
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
        dump_memory(pid);
      }
      /* continue the process */
      _check_if_not_fail("ptrace", ptrace(PTRACE_CONT, pid, NULL, NULL));
    }
  } while( !WIFEXITED(status) && !WIFSIGNALED(status) );

  return 0;
}

/**
 * Read the target process memory
 */
int
read_memory(pid_t pid, long addr, size_t size /* nozero */, unsigned char *buff)
{
  union { long word; unsigned char byte[4]; } word;
  long round = (size >> 2) - (size & 3 ? 0 : 1); /* n of 4 by 4 rounds - 1 */

  for(  ; round-- ; addr += 4, buff += 4 ) {
    errno = 0; /* clear errno first! */
    word.word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
    if( errno != 0 ) return -1;
    *buff     = word.byte[0]; *(buff+1) = word.byte[1];
    *(buff+2) = word.byte[2]; *(buff+3) = word.byte[3];
  }

  errno = 0;
  word.word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
  if( errno != 0 ) return -1;

  /* copy the remaining bytes in the nth round  */
  switch( size & 3 ) {
    case 0: *(buff+3) = word.byte[3];
    case 3: *(buff+2) = word.byte[2];
    case 2: *(buff+1) = word.byte[1];
    case 1: *buff     = word.byte[0];
  }

  return 0;
}

/**
 * Write to the target process memory
 */
int
write_memory(pid_t pid, long addr, size_t size /* nonzero */, const unsigned char *buff)
{
  union { long word; unsigned char byte[4]; } word;
  long round = (size >> 2) - (size & 3 ? 0 : 1); /* n of 4 by 4 rounds - 1 */

  for(  ; round-- ; addr += 4, buff += 4 ) {
    if( ptrace(PTRACE_POKEDATA, pid, addr, *(long *) buff) == -1 ) return -1;
  }

  /* 4 bytes block left */
  if( (size & 3) == 0 ) {
    return ptrace(PTRACE_POKEDATA, pid, addr, *(long *) buff);
  }

  /* < 4 left, read 4 bytes block, and write back the remaining bytes */
  if( read_memory(pid, addr, 4, word.byte) != -1 ) {
    switch( size & 3 ) {
      case 3: word.byte[2] = *(buff+2);
      case 2: word.byte[1] = *(buff+1);
      case 1: word.byte[0] = *buff;
               return ptrace(PTRACE_POKEDATA, pid, addr, word.word);
    }
  }

  return -1;
}

static void
dump_memory(pid_t pid)
{
  struct regs_struct regs;
  char buff[27] = {0};
  int i;

  _check_if_not_fail("ptrace", ptrace(PTRACE_GETREGS, pid, NULL, &regs));

  printf("eax => 0x%08"PRIx32"\n", regs.eax);

  for(i = 1; i < 27 ; i++) {
    _check_if_not_fail("read_process", read_memory(pid, regs.eax, i, (unsigned char *) buff));
    printf("%s\n", buff);
  }

  _check_if_not_fail("write_memory", write_memory(pid, regs.eax, 10, (const unsigned char *) "0123456789"));
}
