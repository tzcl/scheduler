#ifndef __CPU_H_
#define __CPU_H_

#include "priority_queue.h"
#include "process.h"
#include <stdint.h>

/**
 * Stores information about a CPU.
 *
 *   id: the CPU id
 *   active_process: the currently running process
 *   jobs: a queue containing waiting processes
 *   remaining_time: the remaining time for all processes */
typedef struct {
  int id;
  Process *active_process;
  PriorityQueue *jobs;
  Time remaining_time;
} CPU;

/**
 * Initialises the data members of a new CPU and allocates memory.
 *
 *   id: the CPU id */
CPU *new_cpu(int id);

/** Frees the memory associated with a CPU. */
void free_cpu(CPU *cpu);

#endif // __CPU_H_
