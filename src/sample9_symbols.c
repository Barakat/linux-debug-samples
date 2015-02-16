/**
 * @file sample9_symbols.c
 * @brief Resolving the shared object symbols and their
 * offsets in the dynamic table
 * and its loaded shared objects
 * @author Barakat S. 2013 <b4r4k47@hotmail.com>
 */
#include "common.h"

int
main(int argc, char **argv)
{
  const char *const shared = "./target/target7.so";
  int fd;
  struct stat st;
  void *elf;
  Elf32_Ehdr *ehdr;
  Elf32_Shdr *shdr;
  Elf32_Sym *iter_sym, *sym_start, *sym_end;
  char *sym_str;
  int i;

  /* map the library file into the memory */
  _check_if_not_fail("open", (fd = open(shared, O_RDONLY)));
  _check_if_not_fail("fstat", fstat(fd, &st));
  _check_if_not_fail("mmap", (int) (elf = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)));

  /* ehdr: elf header */
  ehdr = (Elf32_Ehdr *) elf;
  /* shdr: section header */
  shdr = (Elf32_Shdr *) (elf + ehdr->e_shoff);
  /* search for the dynamic section, SHT_DYNSYM */
  for( i = 0 ; i < ehdr->e_shnum ; i++ ) {
    if( shdr[i].sh_type == SHT_DYNSYM ) {
      sym_start = (Elf32_Sym *) ( elf + shdr[i].sh_offset );
      sym_end   = (Elf32_Sym *) ( elf + shdr[i].sh_offset + shdr[i].sh_size );
      /* the section index in the string table */
      sym_str   = (char *) ( elf + shdr[shdr[i].sh_link].sh_offset );
      /* iterate over symbols and pick only the functions, STT_FUNC, get their ASCII name
       * from the string table */
      for( iter_sym = sym_start ; iter_sym < sym_end ; iter_sym++ ) {
        if( ELF32_ST_TYPE(iter_sym->st_info) == STT_FUNC )
        printf("0x%08lx %s\n", (long) iter_sym->st_value, sym_str + iter_sym->st_name);
      }
    }
  }

  munmap(elf, st.st_size);
  close(fd);
  return 0;
}
