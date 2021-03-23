#include "priority_queue.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Initialises the data members of a new queue and allocates memory.
 * */
PriorityQueue *new_queue() {
  PriorityQueue *queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  queue->size = 0;
  queue->root = NULL;

  return queue;
}

/**
 * Frees the memory associated with a queue */
void free_queue(PriorityQueue *queue) {
  Node *next_node;
  Node *curr = queue->root;

  while (curr) {
    next_node = curr->next;
    free_node(curr);
    curr = next_node;
  }

  free(queue);
}

/**
 * Initialises the data members of a new Node and allocates memory */
Node *new_node(Process *process) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->process = process;
  node->next = NULL;

  return node;
}

/**
 * Frees the memory associated with a Node. Also frees the associated Process.
 */
void free_node(Node *node) {
  free_process(node->process);
  free(node);
}

/**
 * Checks whether a queue is empty */
bool empty_queue(PriorityQueue *queue) { return queue->size == 0; }

/**
 * Returns the top node of the queue */
Process *top_queue(PriorityQueue *queue) {
  if (!empty_queue(queue))
    return queue->root->process;
  fprintf(stderr, "Critical error! Queue is empty!\n");
  exit(EXIT_FAILURE);
}

/**
 * Helper function that compares the priority of nodes by the priority of their
 * processes. */
bool higher_node_priority(Node *n1, Node *n2) {
  return higher_priority(n1->process, n2->process);
}

/**
 * Adds a new node to the queue */
void push_queue(PriorityQueue *queue, Process *process) {
  Node *new = new_node(process);

  if (empty_queue(queue)) {
    queue->root = new;
  } else {
    /* See if the new node should go at the front of the queue */
    Node *curr = queue->root;
    if (higher_node_priority(new, curr)) {
      printf("%d has lower priority than %d\n", new->process->execution_time,
             curr->process->execution_time);
      queue->root = new;
      new->next = curr;
    } else {
      /* Iterate through nodes until a node has lower priority than new */
      while (curr->next && higher_node_priority(curr, new)) {
        curr = curr->next;
      }
      /* Insert the new node */
      new->next = curr->next;
      curr->next = new;
    }
  }

  queue->size++;
}

/**
 * Removes the top node from the queue */
void pop_queue(PriorityQueue *queue) {
  Node *next = queue->root->next;
  free_node(queue->root);
  queue->root = next;
  queue->size--;
}

// TODO: delete
void print_queue(PriorityQueue *queue) {
  Node *curr = queue->root;

  while (curr) {
    printf("%d: %d; ", curr->process->id, curr->process->execution_time);
    curr = curr->next;
  }

  printf("\n");
}
