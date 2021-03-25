#include "cpu.h"

#include <stdlib.h>

/**
 * Initialises the data members of a new CPU and allocates memory. */
CPU *new_cpu(int id) {
  CPU *cpu = (CPU *)malloc(sizeof(CPU));
  cpu->id = id;
  cpu->active_process = NULL;
  cpu->remaining_time = 0;

  cpu->jobs = new_queue();

  return cpu;
}

/** Frees the memory associated with a CPU. */
void free_cpu(CPU *cpu) {
  if (cpu->active_process)
    free_process(cpu->active_process);

  free_queue(cpu->jobs);
  free(cpu);
}
