/**
 * @file target1.c
 * @brief Testing target 1
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

void
_check_if_not_fail(const char *func, int ret)
{
  if( ret == -1 ) {
    fprintf(stderr, "Error (%s): %s\n", func, strerror(errno));
    exit(-1);
  }
}

void
_check_if_not_null(const char *func, void *ret)
{
  if( ret == NULL ) {
    fprintf(stderr, "Error (%s): %s\n", func, strerror(errno));
    exit(-1);
  }
}
