/**
 * @file target8.c
 * @brief Testing target 8
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/syscall.h>

void
func(void)
{
  void *pputs;

  pputs = dlsym(RTLD_NEXT, "puts");

  printf("puts address => %p\n", pputs);
  __asm__ __volatile__(
    "movl %0, %%eax\n\r"
    "int $3\n\r"
    :
    :"r"(pputs)
  );

  syscall(SYS_write, 0, "Should break now:\n", 18); /* no use for any function, direct write via the kernel */
  puts("succedded!\n"); /* should tarp here */
}

int
main(int argc, char **argv)
{
  func();
  return 0;
}
