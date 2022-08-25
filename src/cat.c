#include <errno.h>
#include <stdio.h>

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
  puts("Usage: cat <FILE>");
}

int main(int argc, char **argv) {
  char buffer[BUFSIZ];
  int status = 0;

  if (argc == 1) {
    usage();
    return 0;
  }

  /* open file to read */
  const char *filename = argv[1];
  FILE *file = fopen(filename, "r");
  ENSURE(file);

  /* get size of file */
  status = fseek(file, 0, SEEK_END);
  ENSURE(!status);
  long int file_size = ftell(file);
  errno = 0;
  rewind(file);
  ENSURE(!errno);

  /* copy from input stream to output stream */
  while (file_size > 0) {
    size_t count = (file_size >= BUFSIZ)
      ? BUFSIZ : file_size;
    status = fread((void *)&buffer, sizeof(char), count, file);
    ENSURE(status > 0);
    status = fwrite((const void *)&buffer, sizeof(char), count, stdout);
    ENSURE(status > 0);
    file_size -= count;
  }

  status = fclose(file);
  ENSURE(!status);

  return 0;
}
