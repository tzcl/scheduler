#include "cpu.h"
#include "list.h"
#include "priority_queue.h"
#include "process.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LEN 100 /* maximum line length of input file */

/**
 * Splits a process into k subprocesses. */
void split_process(Process *process, int k, linked_list_t *parallel);

/**
 * Reads the next process into the given buffer. Returns true if there are any
 * errors.
 *
 *  processes_file: a pointer to the processes file
 *  process: the variable to store the process in */
bool read_next_process(FILE *processes_file, Process *process);

/**
 * Compare CPUs by their total remaining time and break ties using ID. */
int compare_cpus(const void *a, const void *b) {
  /* a and b are pointers to pointers to CPUs */
  CPU *first_cpu = *(CPU **)a;
  CPU *second_cpu = *(CPU **)b;

  return (first_cpu->remaining_time - second_cpu->remaining_time +
          first_cpu->id - second_cpu->id);
}

int main(int argc, char **argv) {
  /* Parse arguments
   * ---------------
   * Allowed flags:
   *   -f filename: specify the name of the file describing the processes
   *   -p processors: specifies the number of processors, where 1 <= N <= 1024
   *   -c: an optional parameter which invokes my custom scheduler */
  bool use_custom_scheduler = false;
  int num_cpus = 1;
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
  /* tracks whether there are any active processes */
  bool active_processes = false;
  uint32_t proc_left = 0;
  Time time = 0;

  /* performance statistics */
  uint32_t num_processes = 0;
  double avg_turnaround = 0;
  double avg_overhead = 0;
  double max_overhead = 0;

  /* create CPUs */
  /* treat this as a sorted array of CPUs? */
  CPU *cpu_array[num_cpus];
  CPU *sorted_cpu_array[num_cpus];
  for (int i = 0; i < num_cpus; i++) {
    cpu_array[i] = new_cpu(i);
    sorted_cpu_array[i] = cpu_array[i];
  }

  /* store which CPUs are the most free */
  // TODO: remove?
  int lowest_cpu_load[num_cpus];
  for (int i = 0; i < num_cpus; i++) {
    lowest_cpu_load[i] = i;
  }

  /* track parallelisable processes */
  linked_list_t *parallel = new_list();

  /* store processes as they are read in */
  Process next_process;
  more_processes = read_next_process(processes_file, &next_process);

  while (more_processes || active_processes) {
    /* process arrives */
    while (more_processes && time == next_process.arrival) {
      if (num_cpus > 1 && next_process.parallelisable) {
        /* split the process into k subprocesses where k <= x (and k <= N) */
        int k = next_process.execution_time;
        if (k > num_cpus)
          k = num_cpus;
        split_process(&next_process, k, parallel);
        for (int i = 0; i < k; i++) {
          /* assign the subprocesses to the k lowest load processors */
          give_process(sorted_cpu_array[i], &next_process, time);
        }
      } else {
        /* give the process to the CPU with the lowest total remaining time */
        give_process(sorted_cpu_array[0], &next_process, time);
      }

      /* increase the number of remaining processes */
      proc_left++;

      /* sort each CPU by total remaining time */
      qsort(&sorted_cpu_array, num_cpus, sizeof(CPU *), &compare_cpus);

      /* read in the next process */
      more_processes = read_next_process(processes_file, &next_process);
    }

    /* Check if any CPU changed process */
    for (int i = 0; i < num_cpus; i++) {
      /* Does this incur performance loss? */
      CPU *cpu = cpu_array[i];
      if (cpu->changed_process) {
        printf("%u,RUNNING,pid=%u", time, cpu->active_process->id);
        if (cpu->active_process->parallelisable)
          printf(".%d", cpu->id);
        printf(",remaining_time=%u,cpu=%d\n",
               cpu->active_process->remaining_time, cpu->id);
        cpu->changed_process = false;
      }
    }

    time++;

    /* Update each CPU by one time step */
    for (int i = 0; i < num_cpus; i++) {
      update_cpu(cpu_array[i]);
    }

    /* Check each CPU to see if any processes finished */
    for (int i = 0; i < num_cpus; i++) {
      CPU *cpu = cpu_array[i];

      if (cpu->finished_process) {
        Process *proc = cpu->active_process;

        if (num_cpus > 1 && proc->parallelisable) {
          node_t *node;
          if (search_list(&node, proc->id, parallel)) {
            node->count--;

            /* still more subprocesses to run, process is not finished */
            /* don't print anything yet */
            if (node->count > 0) {
              /* start the next process on the CPU */
              cpu_next_process(cpu, time);
              cpu->finished_process = false;
              continue;
            }

            /* process is finished, don't need to track subprocesses any more */
            delete_list_node(node, parallel);
          }
        }
        /* process is finished */
        proc_left--;

        /* calculate performance statistics */
        num_processes++;
        unsigned long turnaround = time - proc->arrival;
        double overhead = turnaround / (double)proc->execution_time;

        avg_turnaround += (turnaround - avg_turnaround) / num_processes;
        avg_overhead += (overhead - avg_overhead) / num_processes;
        if (overhead > max_overhead)
          max_overhead = overhead;

        cpu->finished_process = false;
        cpu->print_finished = true;
      }
    }

    /* print any FINISHED events */
    for (int i = 0; i < num_cpus; i++) {
      CPU *cpu = cpu_array[i];
      if (cpu->print_finished) {
        printf("%u,FINISHED,pid=%u,proc_remaining=%u\n", time,
               cpu->active_process->id, proc_left);

        /* start the next process on the CPU */
        cpu_next_process(cpu, time);
        cpu->finished_process = false;

        cpu->print_finished = false;
      }
    }

    /* sort each CPU by total remaining time */
    qsort(&sorted_cpu_array, num_cpus, sizeof(CPU *), &compare_cpus);

    /* check if any CPUs are still running */
    active_processes = false;
    for (int i = 0; i < num_cpus; i++) {
      if (cpu_array[i]->active_process) {
        active_processes = true;
        break;
      }
    }
  }

  /* round off overhead statistics */
  avg_overhead = round(avg_overhead * 100) / 100;
  max_overhead = round(max_overhead * 100) / 100;

  printf("Turnaround time %g\n", round(avg_turnaround));
  printf("Time overhead %g %g\n", max_overhead, avg_overhead);
  printf("Makespan %d\n", time);

  for (int i = 0; i < num_cpus; i++) {
    free_cpu(cpu_array[i]);
  }

  free_list(parallel);

  fclose(processes_file);

  return 0;
}

/**
 * Splits a process into k subprocesses. */
void split_process(Process *process, int k, linked_list_t *parallel) {
  process->remaining_time = ceil(process->execution_time / (double)k) + 1;
  insert_list_node(process->id, k, parallel);
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
