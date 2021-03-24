##
# COMP30023 Project 1
#
# @file
# @version 0.1

allocate: allocate.o process.o priority_queue.o
	gcc -o allocate allocate.o process.o priority_queue.o -Wall -lm

allocate.o: allocate.c process.o priority_queue.o
	gcc -c allocate.c

process.o: process.c process.h
	gcc -c process.c

priority_queue.o: priority_queue.c priority_queue.h
	gcc -c priority_queue.c

clean:
	rm allocate *.o

# end
