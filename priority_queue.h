#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_

#include "process.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * A node in the priority queue.
 *
 *   process: the stored process
 *   next: the next node in the queue
 * */
typedef struct queue_node Node;
struct queue_node {
  Process *process;
  Node *next;
};

/**
 * A priority queue of processes, implemented using a singly-linked list.
 *
 *   size: the number of elements in the queue
 *   root: the root element of the queue
 *  */
typedef struct {
  uint32_t size;
  Node *root;
} PriorityQueue;

/**
 * Initialises the data members of a new queue and allocates memory.
 * */
PriorityQueue *new_queue();

/**
 * Frees the memory associated with a queue */
void free_queue(PriorityQueue *queue);

/**
 * Checks whether a queue is empty. */
bool empty_queue(PriorityQueue *queue);

/**
 * Returns the top node of the queue. */
Process *top_queue(PriorityQueue *queue);

/**
 * Adds a new node to the queue. */
void push_queue(PriorityQueue *queue, Process *process);

/**
 * Removes the top node from the queue. */
void pop_queue(PriorityQueue *queue);

// TODO: delete
void print_queue(PriorityQueue *queue);

#endif // __PRIORITY_QUEUE_H_
