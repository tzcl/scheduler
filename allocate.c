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

/**
 * Changes the pointer to the active process and stores the data of that
 * process.
 *
 *  active_process: a pointer to the active process
 *  curr_process: container for the data of the active process
 *  next_process: the data of the next process
 *  time: the current time of the simulation */
bool change_process(Process **active_process, Process *curr_process,
                    Process *next_process, Time time);

int main(int argc, char **argv) {
  /* Parse arguments
   * ---------------
   * Allowed flags:
   *   -f filename: specify the name of the file describing the processes
   *   -p processors: specifies the number of processors, where 1 <= N <= 1024
   *   -c: an optional parameter which invokes my custom scheduler */
  bool use_custom_scheduler = false;
  int num_processors = 0;
  char *filename = NULL;

  int opt;
  while ((opt = getopt(argc, argv, "f:p:c")) != -1) {
    switch (opt) {
    case 'f':
      filename = optarg;
      break;
    case 'p':
      num_processors = atoi(optarg);
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
  bool changed_process = false;
  Time time = 0; /* time of the simulation */

  /* performance statistics */
  unsigned long num_processes = 0;
  double avg_turnaround = 0;
  double avg_overhead = 0;
  double max_overhead = 0;

  PriorityQueue *queue = new_queue();

  /* need to be able to tell if active_process is set to anything
   * idea: allocate memory for all processes, change new_node to just copy the
   * pointer and make active_process point to a process */
  Process *active_process = NULL;
  Process curr_process, next_process;
  more_processes = read_next_process(processes_file, &next_process);

  while (more_processes || active_process) {
    /* process arrives */
    while (more_processes && time == next_process.arrival) {
      if (active_process) {
        if (higher_priority(&next_process, active_process)) {
          /* copy the active process into the queue */
          push_queue(queue, active_process);

          /* make the new process active */
          changed_process = change_process(&active_process, &curr_process,
                                           &next_process, time);
        } else {
          push_queue(queue, &next_process);
        }
      } else {
        /* make the new process active */
        changed_process =
            change_process(&active_process, &curr_process, &next_process, time);
      }

      more_processes = read_next_process(processes_file, &next_process);
    }

    if (changed_process) {
      printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time,
             active_process->id, active_process->remaining_time, 0);
      changed_process = false;
    }

    time++;

    if (active_process) {
      active_process->remaining_time--;

      if (active_process->remaining_time == 0) {
        /* process is finished */
        printf("%d,FINISHED,pid=%d,proc_remaining=%lu\n", time,
               active_process->id, queue->size);

        num_processes++;
        unsigned long turnaround = time - active_process->arrival;
        double overhead = turnaround / (double)active_process->execution_time;

        avg_turnaround += (turnaround - avg_turnaround) / num_processes;
        avg_overhead += (overhead - avg_overhead) / num_processes;
        if (overhead > max_overhead)
          max_overhead = overhead;

        /* swap active_process to whatever is next */
        if (!empty_queue(queue)) {
          /* How am I going to store whatever is at the top of the queue? */
          changed_process = change_process(&active_process, &curr_process,
                                           top_queue(queue), time);
          pop_queue(queue);
        } else {
          active_process = NULL;
        }
      }
    }
  }

  /* round off overhead statistics */
  avg_overhead = round(avg_overhead * 100) / 100;
  max_overhead = round(max_overhead * 100) / 100;

  printf("Turnaround time %g\n", round(avg_turnaround));
  printf("Time overhead %g %g\n", max_overhead, avg_overhead);
  printf("Makespan %d\n", time);

  free_queue(queue);
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

/**
 * Changes the pointer to the active process and stores the data of that
 * process.
 *
 *  active_process: a pointer to the active process
 *  curr_process: container for the data of the active process
 *  next_process: the data of the next process */
bool change_process(Process **active_process, Process *curr_process,
                    Process *next_process, Time time) {
  store_process(next_process, curr_process);
  curr_process->start_time = time;
  *active_process = curr_process;
  return true;
}
