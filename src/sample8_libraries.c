/**
 * @file sample8_libraries.c
 * @brief Listing the executable virtual memory base address
 * and its loaded shared objects
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

int
main(int argc, char **argv)
{
  char maps[PATH_MAX+1];
  FILE *fh;
  char buffer[PATH_MAX+1];
  char executable[PATH_MAX+1];
  char permissions[5];
  unsigned long baseaddr;

  if( argc < 2 ) {
    printf("usage: %s [target PID]\n", argv[0]);
    return -1;
  }

  /* open the target process /proc/[PID]/maps */
  snprintf(maps, PATH_MAX+1, "/proc/%s/maps", argv[1]);
  _check_if_not_null("fopen", (fh = fopen(maps, "r")));

  /* parse the maps file and look for the executable, r-xp, objects */
  while( fgets(buffer, sizeof(buffer) - 1, fh) ) {
    sscanf(buffer, "%lx-%*x %4s %*x %*x:%*x %*d %s", &baseaddr, permissions, executable);
    if( strcmp(permissions, "r-xp") == 0 && executable[0] == '/' )
      printf("0x%08lx %s\n", baseaddr, executable);
  }

  fclose(fh);
  return 0;
}
