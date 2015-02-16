/**
 * @file target6.c
 * @brief Testing target 6
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include <stdio.h>


int
main(int argc, char **argv)
{
  char p[] = "abcdefghijklmnopqrstuvwxyz";

  printf("p (address)  => %p\n", p);
  __asm__ __volatile__(
    "movl %0, %%eax\n\t"
    "int $3\n\t"
    :
    :"r"(p)
  );
  printf("p (value)  => %s\n", p);
  return 0;
}
