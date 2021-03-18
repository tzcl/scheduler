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
typedef struct Process {
  int id;
  int start_time;
  int execution_time;
  int remaining_time;
  int end_time;
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
void init_process(Process *process, char *line);

// TODO: delete
// This is for testing at this stage (eventually I will need to print output)
void print_process(Process *process);

#endif // __PROCESS_H_
