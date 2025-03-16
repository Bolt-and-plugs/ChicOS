#include "list.h"
#include "../log/log.h"

// include id inside node
i32 max_id;

List *new_list(i32 data_size) {
  List *l;
  if (data_size <= 0)
    return NULL;
  l = malloc(sizeof(List));
  if (l)
    l->head = malloc(data_size);
  if (!l->head)
    return NULL;

  return l;
}

Node *alloc_node(List *l, void *d) {
  if (!l || !d) {
    c_log(ERROR, MEM_STATUS, "list or data invalid!", NULL);
    return NULL;
  }

  Node *n = malloc(l->data_size);
  memcpy(n->data, d, l->data_size);
  return n;
}

void push_list(List *l, void *d) {

  Node *n = l->head;
  while (n)
    n = n->next;

  // need to handle if head

  // handle simple adding
  n->next = alloc_node(l, d);
  n->prev = n;
  l->tail = n->next;
}
void pop_list(List *l, i32 id) {}

void clear_list(List *l) {}
