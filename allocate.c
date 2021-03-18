#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  bool use_custom_scheduler = false;
  int num_processors = 0;
  char *filename = NULL;

  int opt;
  while ((opt = getopt(argc, argv, "f:p:c")) != -1) {
    switch (opt) {
    case 'f':
      filename = optarg;
      break;
    case 'p':
      num_processors = atoi(optarg);
      break;
    case 'c':
      use_custom_scheduler = true;
      break;
    case '?':
      fprintf(stderr, "Usage: %s -%c requires an argument!\n", argv[0], optopt);
      exit(EXIT_FAILURE);
    default:
      fprintf(stderr, "Usage: %s [-fpc]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  FILE *processes_file;
  if ((processes_file = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Error opening file %s!\n", filename);
    exit(EXIT_FAILURE);
  }

  char buf[100];
  while ((buf == fgets(buf, 100, processes_file))) {
    printf("%s", buf);
  }

  fclose(processes_file);

  return 0;
}
