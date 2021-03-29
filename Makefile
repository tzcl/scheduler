##
# COMP30023 Project 1
#

allocate: allocate.o process.o priority_queue.o cpu.o list.o
	gcc -o allocate allocate.o process.o priority_queue.o cpu.o list.o -Wall -lm

allocate.o: allocate.c
	gcc -c allocate.c

priority_queue.o: priority_queue.c priority_queue.h
	gcc -c priority_queue.c

cpu.o: cpu.c cpu.h
	gcc -c cpu.c

process.o: process.c process.h
	gcc -c process.c

list.o: list.c list.h
	gcc -c list.c

.PHONY: clean

clean:
	rm -f allocate *.o

# end
