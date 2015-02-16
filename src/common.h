/**
 * @file common.h
 * @brief The common include header files
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#ifndef __COMMON_H__
#define __COMMON_H

#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/user.h>

void
_check_if_not_fail(const char *func, int ret);

void
_check_if_not_null(const char *func, void *ret);

#endif
