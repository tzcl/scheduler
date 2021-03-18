#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Initialises the data members of a process using a line from the file
 * describing the processes. The line must be of the form:
 * (time-arrived, process-id, execution-time, parallelisable).
 *
 *   process: a pointer to the process to initialise
 *   line: a string of the form specified above
 *  */
void init_process(Process *process, char *line) {
  int start_time = atoi(strtok(line, " "));
  int id = atoi(strtok(NULL, " "));
  int execution_time = atoi(strtok(NULL, " "));
  char *parallelisable = strtok(NULL, " ");

  process->id = id;
  process->start_time = start_time;
  process->execution_time = execution_time;
  process->remaining_time = execution_time;
  process->parallelisable = *parallelisable == 'p';
}

// TODO: delete
// This is for testing at this stage (eventually I will need to print output)
void print_process(Process *process) {
  printf("Process (%c): %d started at %d and has %d remaining (%d)\n",
         process->parallelisable ? 'p' : 'n', process->id, process->start_time,
         process->remaining_time, process->execution_time);
}
