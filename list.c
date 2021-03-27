/* Code from COMP20007. Available at:
 * https://github.com/tzcl/comp20007-workshops/ */

#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

node_t *new_list_node(uint32_t id, int count) {
  node_t *node = malloc(sizeof(node_t));
  assert(node);

  node->id = id;
  node->count = count;
  node->prev = NULL;
  node->next = NULL;

  return node;
}

linked_list_t *new_list() {
  linked_list_t *list = malloc(sizeof(linked_list_t));
  assert(list);

  list->size = 0;
  list->head = NULL;
  list->tail = NULL;

  return list;
}

void free_list(linked_list_t *list) {
  node_t *curr = list->head, *prev;
  while (curr) {
    prev = curr;
    curr = curr->next;
    free(prev);
  }

  free(list);
}

bool empty_list(linked_list_t *list) { return list->size == 0; }

bool search_list(node_t **result, uint32_t id, linked_list_t *list) {
  assert(list && !empty_list(list));

  node_t *curr = list->head;
  while (curr) {
    if (curr->id == id) {
      *result = curr;
      return true;
    }
    curr = curr->next;
  }

  return false;
}

void insert_list_node(uint32_t id, int count, linked_list_t *list) {
  node_t *node = new_list_node(id, count);
  assert(node && list);

  if (!list->head) {
    list->head = node;
  }
  if (list->tail) {
    list->tail->next = node;
    node->prev = list->tail;
  }

  list->tail = node;
  list->size++;
}

void delete_list_node(node_t *node, linked_list_t *list) {
  assert(node && list);

  if (node == list->head) {
    list->head = node->next;
  }
  if (node == list->tail) {
    list->tail = node->prev;
  }
  if (node->prev) {
    node->prev->next = node->next;
  }
  if (node->next) {
    node->next->prev = node->prev;
  }

  free(node);
  list->size--;
}

void print_list(linked_list_t *list) {
  node_t *curr = list->head;
  while (curr) {
    printf("%u: %d; ", curr->id, curr->count);
    curr = curr->next;
  }
  printf("\n");
}
