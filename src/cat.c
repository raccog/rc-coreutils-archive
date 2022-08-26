/*
 * cat - concatenate text to stdout without modification
 *
 * While inspired by Unix cat, it does not provide the same options
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PRE_ERR "ERROR"

#define MAX(a, b) \
  ({int _a = a; \
   int _b = b; \
   _a > _b ? _a : _b;})

#define ENSURE(expr) \
  if (!(expr)) { \
    perror(PRE_ERR); \
    return 1; \
  }

void usage() {
  puts("Usage: cat [OPTION] [FILE]\n"
      "Concatenate text to stdout without modification.\n\n"
      "Stdin is used as input if FILE is - or FILE is empty.\n\n"
      "Options:\n"
      "  -h, --help    display this help message and exit");
}

int main(int argc, char **argv) {
  char buffer[BUFSIZ];
  FILE *file = NULL;
  int status = 0;

  if (argc > 1) {
    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
      usage();
      return 0;
    }
  }

  if (argc < 2 || !strcmp(argv[1], "-")) {
    /* use stdin as input */
    file = stdin;
  } else {
    /* read file as input */
    const char *filename = argv[1];
    file = fopen(filename, "r");
  }
  ENSURE(file);

  /* copy from input stream to output stream */
  while (true) {
    /* read and check for error */
    size_t bytes_read = fread((void *)&buffer, sizeof(char), BUFSIZ, file);
    if (bytes_read != BUFSIZ) {
      ENSURE(!ferror(file));
    }

    /* write and check for error */
    size_t bytes_written = fwrite((const void *)&buffer, sizeof(char),
        bytes_read, stdout);
    ENSURE(bytes_written == bytes_read);

    /* break if at end of file */
    if (feof(file) || bytes_read == 0) {
      break;
    }
  }

  if (file != stdin) {
    status = fclose(file);
    ENSURE(!status);
  }

  return 0;
}
