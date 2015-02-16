# @file target4.s
# @brief Target 4, assembly file
# @author Barakat S. 2013 <b4r4k47@hotmail.com>
#

.globl _start

.data
  value1_float         :  .float    1.1
  value2_double        :  .double   2.2
  .align 16
  value3_long_double   :  .long     343597384     # 3.3
                          .long     -719407023
                          .long     16384
  value4_float         :  .float    4.4
  value5_float         :  .float    5.5
  value6_float         :  .float    6.6
  value7_float         :  .float    7.7
  value8_double        :  .double   8.8
.text

_start:
  # floating point test values

  flds value1_float
  fldl value2_double
  fldt value3_long_double
  flds value4_float
  flds value5_float
  flds value6_float
  flds value7_float
  fldl value8_double

  # breakpoint
  int $3

  # call sys_exit(0)
  movl  $1, %eax
  movl  $0, %ebx
  int   $0x80

