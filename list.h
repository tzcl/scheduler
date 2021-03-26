/* Code from COMP20007. Available at:
 * https://github.com/tzcl/comp20007-workshops/ */

#ifndef __LIST_H_
#define __LIST_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct list_node node_t;
struct list_node {
  uint32_t id;
  int count;
  node_t *prev;
  node_t *next;
};

typedef struct linked_list {
  node_t *head;
  node_t *tail;
  uint32_t size;
} linked_list_t;

linked_list_t *new_list();
void free_list(linked_list_t *list);

bool empty_list(linked_list_t *list);

bool search_list(node_t **result, uint32_t id, linked_list_t *list);
void insert_list_node(uint32_t id, int count, linked_list_t *list);
void delete_list_node(node_t *node, linked_list_t *list);

void print_list(linked_list_t *list);

#endif // __LIST_H_
