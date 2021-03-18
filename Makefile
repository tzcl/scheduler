##
# COMP30023 Project 1
#
# @file
# @version 0.1

allocate: allocate.o process.o 
	gcc -o allocate allocate.o process.o -Wall

allocate.o: allocate.c process.o
	gcc -c allocate.c

process.o: process.c process.h
	gcc -c process.c

clean:
	rm allocate *.o

# end
