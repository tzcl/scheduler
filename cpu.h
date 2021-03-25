#ifndef __CPU_H_
#define __CPU_H_

#include "priority_queue.h"
#include "process.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * Stores information about a CPU.
 *
 *   id: the CPU id
 *   active_process: the currently running process
 *   curr_process: stores the currently running process
 *   changed_process: stores whether the CPU changed process
 *   finished_process: stores whether the CPU finished a process
 *   queue: a queue containing waiting processes
 *   remaining_time: the remaining time for all processes
 *  */
typedef struct {
  int id;
  Process *active_process;
  Process curr_process;
  bool changed_process;
  bool finished_process;
  PriorityQueue *queue;
  Time remaining_time;
} CPU;

/**
 * Initialises the data members of a new CPU and allocates memory.
 *
 *   id: the CPU id */
CPU *new_cpu(int id);

/** Frees the memory associated with a CPU. */
void free_cpu(CPU *cpu);

/** Gives the process to the CPU. */
void give_process(CPU *cpu, Process *process, Time time);

/** Set the active process to the next process in the queue.*/
void cpu_next_process(CPU *cpu, Time time);

/** Updates the CPU by one time unit. */
void update_cpu(CPU *cpu);

#endif // __CPU_H_
