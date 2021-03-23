#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Initialises the data members of a process using a line from the file
 * describing the processes. The line must be of the form:
 * (time-arrived, process-id, execution-time, parallelisable).
 *
 *   line: a string of the form specified above
 *  */
Process *new_process(char *line) {
  Process *process = (Process *)malloc(sizeof(Process));
  init_process(process, line);

  return process;
}

/**
 * Initialises a process by copying the values from another process */
Process *copy_process(Process *other) {
  Process *process = (Process *)malloc(sizeof(Process));

  process->id = other->id;
  process->arrival = other->arrival;
  process->execution_time = other->execution_time;
  process->remaining_time = other->execution_time;
  process->parallelisable = other->parallelisable;

  return process;
}

/**
 * Set the data members of a process using a line from the file
 * describing the processes. The line must be of the form:
 * (time-arrived, process-id, execution-time, parallelisable).
 *
 *   process: a pointer to the process to write values to
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

/**
 * Frees the memory associated with a process */
void free_process(Process *process) { free(process); }

/**
 * Compares two processes, p1 and p2. A process has higher priority if it has a
 * lower execution time or lower id if the execution times are equal.
 *
 * Returns true if p1 has higher priority than p2 and vice versa. */
bool higher_priority(Process *p1, Process *p2) {
  if (p1->execution_time == p2->execution_time) {
    return p1->id < p2->id;
  }

  return p1->execution_time < p2->execution_time;
}

// TODO: delete
// This is for testing at this stage (eventually I will need to print output)
void print_process(Process *process) {
  printf("Process (%c): %d started at %d and has %d remaining (%d)\n",
         process->parallelisable ? 'p' : 'n', process->id, process->start_time,
         process->remaining_time, process->execution_time);
}
