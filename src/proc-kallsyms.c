// This file is part of KASLD - https://github.com/bcoles/kasld
//
// Retrieve kernel _stext symbol from /proc/kallsyms
//
// Requires:
// - kernel.kptr_restrict = 0 (Default on Debian <= 9 systems)
//
// Based on original code by spender:
// https://grsecurity.net/~spender/exploits/exploit.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kasld.h"

unsigned long get_kernel_sym(char *name) {
  FILE *f;
  unsigned long addr = 0;
  char dummy;
  char sname[256];
  const char *path = "/proc/kallsyms";

  printf("[.] trying %s...\n", path);

  f = fopen(path, "r");

  if (f == NULL) {
    printf("[-] open/read(%s): %m\n", path);
    return 0;
  }

  int ret = 0;
  while (ret != EOF) {
    ret = fscanf(f, "%p %c %s\n", (void **)&addr, &dummy, sname);

    if (ret == 0)
      continue;

    if (!strcmp(name, sname))
      break;

    addr = 0;
  }

  fclose(f);

  if (addr == 0)
    printf("[-] kernel symbol '%s' not found in %s\n", name, path);

  return addr;
}

int main(int argc, char **argv) {
  unsigned long addr = get_kernel_sym("_stext");
  if (!addr)
    return 1;

  printf("kernel text start: %lx\n", addr);
  printf("possible kernel base: %lx\n", addr &~ KERNEL_BASE_MASK);

  return 0;
}