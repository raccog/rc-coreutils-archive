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

/* contatenate input stream to stdout */
int concat_stream(FILE *stream) {
  char buffer[BUFSIZ];

  /* copy from input stream to output stream */
  while (true) {
    /* read and check for error */
    size_t bytes_read = fread((void *)&buffer, sizeof(char), BUFSIZ, stream);
    if (bytes_read != BUFSIZ) {
      if (ferror(stream)) {
        return 1;
      }
    }

    /* write and check for error */
    size_t bytes_written = fwrite((const void *)&buffer, sizeof(char),
        bytes_read, stdout);
    if (bytes_written != bytes_read) {
      return 1;
    }

    /* break if at end of stream */
    if (feof(stream) || bytes_read == 0) {
      break;
    }
  }
  
  return 0;
}

int main(int argc, char **argv) {
  int status = 0;

  if (argc > 1) {
    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
      usage();
      return 0;
    }
  }

  /* use stdin as input stream if there are no arguments */
  if (argc < 2) {
    status = concat_stream(stdin);
    ENSURE(!status);
    return 0;
  }

  /* concatenate each input stream */
  for (int i = 1; i < argc; ++i) {
    FILE *stream = NULL;

    if (!strcmp(argv[i], "-")) {
      /* use stdin as input */
      stream = stdin;
    } else {
      /* read file as input */
      const char *filename = argv[i];
      stream = fopen(filename, "r");
    }
    ENSURE(stream);
    
    status = concat_stream(stream);
    ENSURE(!status);

    if (stream != stdin) {
      status = fclose(stream);
      ENSURE(!status);
    }
  }

  return 0;
}
