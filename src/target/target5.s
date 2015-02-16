# @file target5.s
# @brief Target 4, assembly file
# @author Barakat S. 2013 <b4r4k47@hotmail.com>
#

.globl _start

.data
  value1  : .byte  0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88

  value2 : .long 0x11111111
           .long 0x22222222
  value3 : .long 0x33333333
           .long 0x44444444

  value4 : .float 11.11
           .float 22.22
.text

_start:
  # mmx test values
  movq value1, %mm0
  movq value2, %mm1
  movq value3, %mm2
  movq value4, %mm3


  # breakpoint
  int $3

  # call sys_exit(0)
  movl  $1, %eax
  movl  $0, %ebx
  int   $0x80

