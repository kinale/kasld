// This file is part of KASLD - https://github.com/bcoles/kasld
//
// Check kernel command line /proc/cmdline for nokaslr flag.
//
// References:
// https://www.kernel.org/doc/html/v6.1/admin-guide/kernel-parameters.html
// ---
// <bcoles@gmail.com>

#include "include/kasld.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long get_kernel_addr_cmdline() {
  FILE *f;
  const char *path = "/proc/cmdline";
  const char *flag = "nokaslr";
  char cmdline[1024];

  printf("[.] trying %s ...\n", path);

  f = fopen(path, "rb");
  if (f == NULL) {
    perror("[-] fopen");
    return 0;
  }

  if (fgets(cmdline, sizeof(cmdline), f) == NULL) {
    perror("[-] fgets");
    fclose(f);
    return 0;
  }

  fclose(f);

  if (strstr(cmdline, flag) == NULL) {
    fprintf(stderr, "[-] Kernel was not booted with nokaslr flag.\n");
    return 0;
  }

  printf("[.] Kernel booted with nokaslr flag.\n");

  return (unsigned long)KERNEL_TEXT_DEFAULT;
}

int main() {
  unsigned long addr = get_kernel_addr_cmdline();
  if (!addr)
    return 1;

  printf("common default kernel text for arch: %lx\n", addr);

  return 0;
}
