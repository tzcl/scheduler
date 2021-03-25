#ifndef __PROCESS_H_
#define __PROCESS_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * Times can be in the range [0, 2^32) */
typedef uint32_t Time;

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
typedef struct {
  uint32_t id;
  Time arrival;
  Time execution_time;
  Time remaining_time;
  Time start_time;
  Time end_time;
  bool parallelisable;
} Process;

/**
 * Initialises the data members of a process using a line from the file
 * describing the processes. The line must be of the form:
 * (time-arrived, process-id, execution-time, parallelisable).
 *
 *   line: a string of the form specified above
 *  */
Process *new_process(char *line);

/**
 * Initialises a process by copying the values from another process. */
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
 * Stores the values of one process in another process.
 *
 *  src: the process to read from
 *  dst: the process to write to */
void store_process(Process *src, Process *dst);

/**
 * Frees the memory associated with a process. */
void free_process(Process *process);

/**
 * Compares two processes, p1 and p2. A process has higher priority if it has a
 * lower execution time or lower id if the execution times are equal.
 *
 * Returns true if p1 has higher priority than p2 and vice versa. */
bool higher_priority(Process *p1, Process *p2);

/**
 * Simulates running the given process for one time step. Prints out the status
 * of the process.
 *
 *   process: the process to simulate
 *   time: the current time of the simulation
 *   cpu: the id of the CPU the process is running on
 * */
void run_process(Process *process, unsigned int time, int cpu);

// TODO: delete
// This is for testing at this stage (eventually I will need to print output)
void print_process(Process *process);

#endif // __PROCESS_H_
