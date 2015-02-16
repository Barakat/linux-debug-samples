# @file target3.s
# @brief Target 3, assembly file
# @author Barakat S. 2013 <b4r4k47@hotmail.com>
#

.globl _start

.data

.text

_start:
  # some of the general purpose registers
  movl $0x123456ff, %eax
  movl $2, %ebx
  movl $3, %ecx

  movl %cs, %esi

  movl $0x11223344, %esp
  sub %edx, %edx        # zero flag  => 1
  stc                   # carry flag => 1

  # movb $+127, %al     # overflow => 1
  # addb $1, %al        #

  # the debugger will change the exit status to 123
  movl $0, %ebx

  # breakpoint
  int $3

  # call sys_exit(ebx)
  movl $1, %eax

  int  $0x80
