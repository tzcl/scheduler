#ifndef __PROCESS_H_
#define __PROCESS_H_

#include <stdbool.h>

/**
 * Stores information about a process.
 *
 *   id: the process id
 *   start_time: when the process arrived
 *   execution_time: how long the process will take to run
 *   remaining_time: the remaining time the process has to run
 *   end_time: when the process finished
 *   parallelisable: whether the process can be parallelised or not
 *  */
typedef struct process {
  unsigned int id;
  unsigned int start_time;
  unsigned int execution_time;
  unsigned int remaining_time;
  unsigned int end_time;
  bool parallelisable;
} Process;

/**
 * Initialises the data members of a process using a line from the file
 * describing the processes. The line must be of the form:
 * (time-arrived, process-id, execution-time, parallelisable).
 *
 *   process: a pointer to the process to initialise
 *   line: a string of the form specified above
 *  */
Process *new_process(char *line);

/**
 * Initialises a process by copying the values from another process */
Process *copy_process(Process *other);
/**
 * Set the data members of a process using a line from the file
 * describing the processes. The line must be of the form:
 * (time-arrived, process-id, execution-time, parallelisable).
 *
 *   process: a pointer to the process to write values to
 *   line: a string of the form specified above
 *  */
void init_process(Process *process, char *line);

/**
 * Frees the memory associated with a process */
void free_process(Process *process);

/**
 * Compares two processes, p1 and p2. A process has higher priority if it has a
 * lower execution time or lower id if the execution times are equal.
 *
 * Returns true if p1 has higher priority than p2 and vice versa. */
bool higher_priority(Process *p1, Process *p2);

// TODO: delete
// This is for testing at this stage (eventually I will need to print output)
void print_process(Process *process);

#endif // __PROCESS_H_
