# Scheduler

Basic scheduler written in C that simulates assigning CPU time to parallelisable and non-parallelisable processes. Deciding which process to run is done using the shortest-time-remaining algorithm.

## Building and running

You can build the scheduler using the provided Makefile. An executable named `allocate` is produced which takes the following command line arguments:
```
usage: ./allocate -f processes.txt -p 1

-f [filename]: the name of the file describing the processes
-p [processors]: the number of processors, 1 <= N <= 1024
```

## Sample output
Given `processes.txt`:
```
0 4 30 n
3 2 40 n
5 1 20 n
20 3 30 n
```
the scheduler will output:
```
0,RUNNING,pid=4,remaining_time=30,cpu=0
5,RUNNING,pid=1,remaining_time=20,cpu=0
25,FINISHED,pid=1,proc_remaining=3
25,RUNNING,pid=4,remaining_time=25,cpu=0
50,FINISHED,pid=4,proc_remaining=2
50,RUNNING,pid=3,remaining_time=30,cpu=0
80,FINISHED,pid=3,proc_remaining=1
80,RUNNING,pid=2,remaining_time=40,cpu=0
120,FINISHED,pid=2,proc_remaining=0
Turnaround time 62
Time overhead 2.93 1.90
Makespan 120
```
