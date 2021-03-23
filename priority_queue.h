#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_

#include "process.h"
#include <stdbool.h>

/**
 * A node in the priority queue.
 *
 *   process: the stored process
 *   next: the next node in the queue
 * */
struct node {
  Process *process;
  struct node *next;
};
typedef struct node Node;

/**
 * A priority queue of processes, implemented using a singly-linked list.
 *
 *   size: the number of elements in the queue
 *   root: the root element of the queue
 *  */
typedef struct {
  int size;
  Node *root;
} PriorityQueue;

/**
 * Initialises the data members of a new queue and allocates memory
 * */
PriorityQueue *new_queue();

/**
 * Frees the memory associated with a queue */
void free_queue(PriorityQueue *queue);

/**
 * Initialises the data members of a new node and allocates memory */
Node *new_node(Process *process);

/**
 * Frees the memory associated with a node */
void free_node(Node *node);

/**
 * Checks whether a queue is empty */
bool empty_queue(PriorityQueue *queue);

/**
 * Returns the top node of the queue */
Process *top_queue(PriorityQueue *queue);

/**
 * Adds a new node to the queue */
void push_queue(PriorityQueue *queue, Process *process);

/**
 * Removes the top node from the queue */
void pop_queue(PriorityQueue *queue);

#endif // __PRIORITY_QUEUE_H_
