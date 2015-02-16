/**
 * @file sample5_mmx_and_3dnow.c
 * @brief Reading MMX registers
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

static void
dump_registers(pid_t pid);

int
main(int argc, char **argv)
{
  const char *const target = "./target/target5";
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

struct regs_mmx_struct
{
  long int cwd;
  long int swd;
  long int twd;
  long int fip;
  long int fcs;
  long int foo;
  long int fos;
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm0;
    uint16_t padding;
  } __attribute__((packed));
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm1;
    uint16_t padding;
  } __attribute__((packed));
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm2;
    uint16_t padding;
  } __attribute__((packed));
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm3;
    uint16_t padding;
  } __attribute__((packed));
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm4;
    uint16_t padding;
  } __attribute__((packed));
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm5;
    uint16_t padding;
  } __attribute__((packed));
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm6;
    uint16_t padding;
  } __attribute__((packed));
  struct {
    union {
      uint8_t  byte[8];
      uint16_t word[4];
      uint32_t dword[2];
      uint64_t qword;
      float floatv[2];
    } mm7;
    uint16_t padding;
  } __attribute__((packed));
};


static void
dump_registers(pid_t pid)
{
  struct regs_mmx_struct mmx;
  int i;

  ptrace(PTRACE_GETFPREGS, pid, NULL, &mmx);

  printf("mm0 (uint8_t)  = {");
  for(i = 0 ; i < 8 ; i++) {
    printf("0x%02"PRIx8", ", mmx.mm0.byte[i]);
  }
  printf("\b\b};\n");

  printf("mm1 (uint32_t) = {0x%08"PRIx32", 0x%08"PRIx32"};\n", mmx.mm1.dword[0], mmx.mm1.dword[1]);
  printf("mm2 (uint64_t) = {0x%016"PRIx64"};\n", mmx.mm2.qword);
  printf("mm3 (float)    = {%f, %f};\n", mmx.mm3.floatv[0] , mmx.mm3.floatv[1]);
}
