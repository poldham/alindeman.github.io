#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s path\n", argv[0]);
    return 1;
  }

  char resolved[PATH_MAX];
  if (realpath(argv[1], resolved)) {
    printf("%s\n", resolved);
  } else {
    err(1, NULL);
  }

  return 0;
}
