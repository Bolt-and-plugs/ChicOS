#ifndef _LIST
#define _LIST

#include "../../defines.h"

typedef struct Node Node;
typedef struct List List;

struct Node {
  i32 id;
  void *data;
  Node *next;
  Node *prev;
};

struct List {
  Node *head;
  Node *tail;
  i32 data_size;
};

List *new_list(i32 data_size);

Node *alloc_node(List *L, void *d);
void push_list(List *l, void *d);
void pop_list(List *l, i32 id);

void clear_list(List *l);

#endif
