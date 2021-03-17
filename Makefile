##
# COMP30023 Project 1
#
# @file
# @version 0.1

allocate: allocate.c
	gcc -o allocate allocate.c

clean:
	rm *.o allocate

# end
