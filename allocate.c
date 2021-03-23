#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "process.h"

int main(int argc, char **argv) {
  /* Parse arguments
   * ---------------
   * Allowed flags:
   *   -f filename: specify the name of the file describing the processes
   *   -p processors: specifies the number of processors, where 1 <= N <= 1024
   *   -c: an optional parameter which invokes my custom scheduler */
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

  /* Open the processes file  */
  FILE *processes_file;
  if (!filename) {
    fprintf(stderr, "Path to processes file missing!\n");
    exit(EXIT_FAILURE);
  }
  if (!(processes_file = fopen(filename, "r"))) {
    fprintf(stderr, "Error opening file %s!\n", filename);
    exit(EXIT_FAILURE);
  }

  char buf[100];
  /* Process process; */
  /* while ((buf == fgets(buf, 100, processes_file))) { */
  /*   init_process(&process, &buf[0]); */
  /*   print_process(&process); */
  /* } */

  PriorityQueue *queue = new_queue();
  fgets(buf, 100, processes_file);
  Process *process = new_process(&buf[0]);
  print_process(process);

  fgets(buf, 100, processes_file);
  Process *process2 = new_process(&buf[0]);
  print_process(process2);

  fgets(buf, 100, processes_file);
  Process *process3 = new_process(&buf[0]);
  print_process(process3);

  push_queue(queue, process);
  printf("top: %d, %d\n", top_queue(queue)->id, queue->size);

  push_queue(queue, process2);
  push_queue(queue, process3);
  printf("top now: %d, %d\n", top_queue(queue)->id, queue->size);

  /* pop_queue(queue); */
  /* pop_queue(queue); */
  /* pop_queue(queue); */
  free_queue(queue);

  fclose(processes_file);

  return 0;
}
