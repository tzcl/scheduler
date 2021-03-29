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
 * Initialises a process by copying the values from another process. */
Process *copy_process(Process *other) {
  Process *process = (Process *)malloc(sizeof(Process));
  store_process(other, process);
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
  Time arrival = strtoul(strtok(line, " "), NULL, 10);
  uint32_t id = strtoul(strtok(NULL, " "), NULL, 10);
  Time execution_time = strtoul(strtok(NULL, " "), NULL, 10);
  char *parallelisable = strtok(NULL, " ");

  process->id = id;
  process->arrival = arrival;
  process->execution_time = execution_time;
  process->remaining_time = execution_time;
  process->parallelisable = *parallelisable == 'p' && execution_time > 1;
}

/**
 * Stores the values of one process in another process.
 *
 *  src: the process to read from
 *  dst: the process to write to */
void store_process(Process *src, Process *dst) {
  dst->id = src->id;
  dst->arrival = src->arrival;
  dst->execution_time = src->execution_time;
  dst->remaining_time = src->remaining_time;
  dst->parallelisable = src->parallelisable;
}

/**
 * Frees the memory associated with a process. */
void free_process(Process *process) { free(process); }

/**
 * Compares two processes, p1 and p2. A process has higher priority if it has
 * remaining time or lower id if the remaining times are equal.
 *
 * Returns true if p1 has higher priority than p2 and vice versa. */
bool higher_priority(Process *p1, Process *p2) {
  if (p1->remaining_time == p2->remaining_time) {
    return p1->id < p2->id;
  }

  return p1->remaining_time < p2->remaining_time;
}
