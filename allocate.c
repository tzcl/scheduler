#include "cpu.h"
#include "priority_queue.h"
#include "process.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LEN 100

/**
 * Reads the next process into the given buffer. Returns true if there are any
 * errors.
 *
 *  processes_file: a pointer to the processes file
 *  process: the variable to store the process in */
bool read_next_process(FILE *processes_file, Process *process);

int main(int argc, char **argv) {
  /* Parse arguments
   * ---------------
   * Allowed flags:
   *   -f filename: specify the name of the file describing the processes
   *   -p processors: specifies the number of processors, where 1 <= N <= 1024
   *   -c: an optional parameter which invokes my custom scheduler */
  bool use_custom_scheduler = false;
  int num_cpus = 0;
  char *filename = NULL;

  int opt;
  while ((opt = getopt(argc, argv, "f:p:c")) != -1) {
    switch (opt) {
    case 'f':
      filename = optarg;
      break;
    case 'p':
      num_cpus = atoi(optarg);
      break;
    case 'c':
      use_custom_scheduler = true;
      break;
    case '?':
      fprintf(stderr, "Usage: %s -%c requires an argument!\n", argv[0], optopt);
      exit(EXIT_FAILURE);
    default:
      fprintf(stderr, "Usage: %s [-fpc]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  /* Open the processes file  */
  FILE *processes_file;
  if (!filename) {
    fprintf(stderr, "Path to processes file missing!\n");
    exit(EXIT_FAILURE);
  }
  if (!(processes_file = fopen(filename, "r"))) {
    fprintf(stderr, "Error opening file %s!\n", filename);
    exit(EXIT_FAILURE);
  }

  /* tracks whether we've seen all processes in the file */
  bool more_processes = true;
  uint32_t proc_left = 0;
  Time time = 0;

  /* performance statistics */
  uint32_t num_processes = 0;
  double avg_turnaround = 0;
  double avg_overhead = 0;
  double max_overhead = 0;

  /* create CPUs */
  CPU *cpu0 = new_cpu(0);
  CPU *cpu1 = new_cpu(1);
  /* will need this eventually */
  /* CPU cpus[num_processors];  */

  Process next_process;
  more_processes = read_next_process(processes_file, &next_process);

  while (more_processes || cpu0->active_process || cpu1->active_process) {
    /* process arrives */
    while (more_processes && time == next_process.arrival) {
      if (cpu0->remaining_time <= cpu1->remaining_time) {
        give_process(cpu0, &next_process, time);
      } else {
        give_process(cpu1, &next_process, time);
      }

      proc_left++;

      /* read in the next process */
      more_processes = read_next_process(processes_file, &next_process);
    }

    /* Update this to run on an array of CPUs */
    /* This should be its own function */
    if (cpu0->changed_process) {
      printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time,
             cpu0->active_process->id, cpu0->active_process->remaining_time,
             cpu0->id);
      cpu0->changed_process = false;
    }
    if (cpu1->changed_process) {
      printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time,
             cpu1->active_process->id, cpu1->active_process->remaining_time,
             cpu1->id);
      cpu1->changed_process = false;
    }

    time++;

    update_cpu(cpu0);
    update_cpu(cpu1);

    if (cpu0->finished_process) {
      printf("%d,FINISHED,pid=%d,proc_remaining=%u\n", time,
             cpu0->active_process->id, proc_left - 1);
      /* not cpu->queue! should print the total processes left */

      proc_left--;

      /* calculate performance statistics! */
      num_processes++;
      unsigned long turnaround = time - cpu0->active_process->arrival;
      double overhead =
          turnaround / (double)cpu0->active_process->execution_time;

      avg_turnaround += (turnaround - avg_turnaround) / num_processes;
      avg_overhead += (overhead - avg_overhead) / num_processes;
      if (overhead > max_overhead)
        max_overhead = overhead;

      cpu_next_process(cpu0, time);

      cpu0->finished_process = false;
    }
    if (cpu1->finished_process) {
      printf("%d,FINISHED,pid=%u,proc_remaining=%u\n", time,
             cpu1->active_process->id, proc_left - 1);

      proc_left--;

      /* calculate performance statistics */
      /* how to track parallelised processes??? */
      num_processes++;
      unsigned long turnaround = time - cpu1->active_process->arrival;
      double overhead =
          turnaround / (double)cpu1->active_process->execution_time;

      avg_turnaround += (turnaround - avg_turnaround) / num_processes;
      avg_overhead += (overhead - avg_overhead) / num_processes;
      if (overhead > max_overhead)
        max_overhead = overhead;

      cpu_next_process(cpu1, time);
      cpu1->finished_process = false;
    }
  }

  /* round off overhead statistics */
  avg_overhead = round(avg_overhead * 100) / 100;
  max_overhead = round(max_overhead * 100) / 100;

  printf("Turnaround time %g\n", round(avg_turnaround));
  printf("Time overhead %g %g\n", max_overhead, avg_overhead);
  printf("Makespan %d\n", time);

  free_cpu(cpu0);
  free_cpu(cpu1);
  fclose(processes_file);

  return 0;
}

/**
 * Reads the next process into the given buffer. Returns true if there are any
 * errors.
 *
 *  processes_file: a pointer to the processes file
 *  process: the variable to store the process in */
bool read_next_process(FILE *processes_file, Process *process) {
  char buffer[MAX_LEN];
  if ((buffer != fgets(buffer, MAX_LEN, processes_file))) {
    return false;
  }
  init_process(process, &buffer[0]);
  return true;
}
