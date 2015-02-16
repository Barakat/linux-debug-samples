/**
 * @file memory_and_registers.h
 * @brief Memory and registers reading and modifying common header.
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#ifndef __MEMORY_AND_REGISTERS_H__
#define __MEMORY_AND_REGISTERS_H__

int
read_memory(pid_t pid, long addr, size_t size /* nozero */, unsigned char *buff);

int
write_memory(pid_t pid, long addr, size_t size /* nonzero */, const unsigned char *buff);

/* general registers */
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



#endif
