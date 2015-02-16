/**
 * @file target1.c
 * @brief Testing target 1
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include <stdio.h>

int
main(int argc, char **argv)
{
  puts("Hello world!");
  __asm__ __volatile__("int $3\n\t");
  return 0;
}
