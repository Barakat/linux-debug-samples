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
  const char *const target = "./target/target3";
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


/* better layout of "struct user_regs_struct" */
struct regs_struct {
  union {         /* anonymous union/struct */
    uint32_t ebx;
    union {
      uint16_t bx;
      struct {
        uint8_t bl;
        uint8_t bh;
      };
    };
  } __attribute__((packed)); /* no padded bytes in the structure */
  union {
    uint32_t ecx;
    union {
      uint16_t cx;
      struct {
        uint8_t cl;
        uint8_t ch;
      };
    };
  } __attribute__((packed));
  union {
    uint32_t edx;
    union {
      uint16_t dx;
      struct {
        uint8_t dl;
        uint8_t dh;
      };
    };
  } __attribute__((packed));
  uint32_t esi;
  uint32_t edi;
  uint32_t ebp;
  union {
    uint32_t eax;
    union {
      uint16_t ax;
      struct {
        uint8_t al;
        uint8_t ah;
      };
    };
  } __attribute__((packed));
  uint16_t ds __attribute__((aligned(4))); /* aligned on 4 bytes, 2 bytes + 2 padding = 4 */
  uint16_t es __attribute__((aligned(4)));
  uint16_t fs __attribute__((aligned(4)));
  uint16_t gs __attribute__((aligned(4)));
    union {
    uint32_t orig_eax;
    union {
      uint16_t orig_ax;
      struct {
        uint8_t orig_al;
        uint8_t orig_ah;
      };
    };
  } __attribute__((packed));
  uint32_t eip;
  uint16_t cs __attribute__((aligned(4)));
  struct {
    uint32_t cf       :1;  /* carry flag */
    uint32_t reserved1:1;  /* reserved */
    uint32_t pf       :1;  /* parity flag */
    uint32_t reserved2:1;  /* reserved */
    uint32_t af       :1;  /* adjust flag */
    uint32_t reserved3:1;  /* reserved */
    uint32_t zf       :1;  /* zero flag */
    uint32_t sf       :1;  /* sign flag */
    uint32_t tf       :1;  /* trap flag */
    uint32_t _if      :1;  /* interruption flag; can't use if */
    uint32_t df       :1;  /* direction flag */
    uint32_t of       :1;  /* overflow flag */
    uint32_t iopl     :2;  /* IO privilege level, 2 bits */
    uint32_t nt       :1;  /* nested task flag */
    uint32_t reserved4:1;  /* reserved */
    uint32_t rf       :1;  /* resume flag */
    uint32_t vm       :1;  /* virtual 8086 mode flag */
    uint32_t ac       :1;  /* alignment check */
    uint32_t vif      :1;  /* virtual interrupt flag */
    uint32_t vip      :1;  /* virtual interrupt pending flag */
    uint32_t id       :1;  /* identification flag */
    uint32_t reserved5:10; /* the rest 10 bits are reserved */   
  } eflags;
  uint32_t esp;
  uint16_t ss __attribute__((aligned(4)));
};


static void
dump_registers(pid_t pid)
{
  struct regs_struct regs;

  _check_if_not_fail("ptrace", ptrace(PTRACE_GETREGS, pid, NULL, &regs));

  printf("eax => 0x%08"PRIx32", ax => 0x%04"PRIx16", ah => 0x%02"PRIx8", al => 0x%02"PRIx8"\n", regs.eax, regs.ax, regs.ah, regs.al);
  printf("ebx => 0x%08"PRIx32", bx => 0x%04"PRIx16", bh => 0x%02"PRIx8", bl => 0x%02"PRIx8"\n", regs.ebx, regs.bx, regs.bh, regs.bl);
  printf("ecx => 0x%08"PRIx32", cx => 0x%04"PRIx16", ch => 0x%02"PRIx8", cl => 0x%02"PRIx8"\n", regs.ecx, regs.cx, regs.ch, regs.cl);
  printf("edx => 0x%08"PRIx32", dx => 0x%04"PRIx16", dh => 0x%02"PRIx8", dl => 0x%02"PRIx8"\n", regs.edx, regs.dx, regs.dh, regs.dl);

  printf("eip => 0x%08"PRIx32"\n", regs.eip);
  printf("esi => 0x%08"PRIx32"\n", regs.esi);
  printf("esp => 0x%08"PRIx32"\n", regs.esp);

  printf("cs  => 0x%04"PRIx16"\n", regs.cs);
  printf("gs  => 0x%04"PRIx16"\n", regs.gs);

  printf("zf => %d, cf => %d, pf => %d, if => %d\n", regs.eflags.zf, regs.eflags.cf, regs.eflags.pf, regs.eflags._if);

  regs.ebx = 123;
  _check_if_not_fail("ptrace", ptrace(PTRACE_SETREGS, pid, NULL, &regs));
}
