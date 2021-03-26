#include "cpu.h"

#include <stdlib.h>

/**
 * Initialises the data members of a new CPU and allocates memory. */
CPU *new_cpu(int id) {
  CPU *cpu = (CPU *)malloc(sizeof(CPU));
  cpu->id = id;
  cpu->active_process = NULL; /* cpu->curr_process is empty */
  cpu->changed_process = false;
  cpu->finished_process = false;
  cpu->remaining_time = 0;

  cpu->queue = new_queue();

  return cpu;
}

/** Frees the memory associated with a CPU. */
void free_cpu(CPU *cpu) {
  free_queue(cpu->queue);
  free(cpu);
}

/**
 * Changes the pointer to the active process and stores the data of that
 * process.
 *
 *  cpu: the CPU whose process to change
 *  next_process: the data of the next process
 *  time: the current time of the simulation */
void cpu_set_active_process(CPU *cpu, Process *next_process, Time time) {
  store_process(next_process, &cpu->curr_process);
  cpu->curr_process.start_time = time;
  cpu->active_process = &cpu->curr_process;
  cpu->changed_process = true;
}

/** Gives the process to the CPU. */
void give_process(CPU *cpu, Process *process, Time time) {
  cpu->remaining_time += process->remaining_time;

  /* check if the CPU already has a running process */
  if (cpu->active_process) {
    if (higher_priority(process, cpu->active_process)) {
      /* put the active process in the queue */
      push_queue(cpu->queue, cpu->active_process);

      /* make the new process active */
      cpu_set_active_process(cpu, process, time);
    } else {
      /* put the new process in the queue */
      push_queue(cpu->queue, process);
    }
  } else {
    /* make the new process the active process */
    cpu_set_active_process(cpu, process, time);
  }
}

/** Set the active process to the next process in the queue.*/
void cpu_next_process(CPU *cpu, Time time) {
  if (!empty_queue(cpu->queue)) {
    cpu_set_active_process(cpu, top_queue(cpu->queue), time);
    pop_queue(cpu->queue);
  } else {
    cpu->active_process = NULL;
  }
}

/** Updates the CPU by one time unit. */
void update_cpu(CPU *cpu) {
  if (cpu->active_process) {
    cpu->active_process->remaining_time--;
    cpu->remaining_time--;

    if (cpu->active_process->remaining_time == 0) {
      cpu->finished_process = true;
    }
  }
}
