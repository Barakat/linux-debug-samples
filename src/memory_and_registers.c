/**
 * @file memory_and_registers.c
 * @brief Memory and registers reading and modifying common functions.
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"
#include "memory_and_registers.h"

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


