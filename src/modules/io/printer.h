#include "../../defines.h"

typedef struct print_list print_list;

struct print_list {
  char print_args[4096];
  print_list *prox;
};

void init_print_queue();
void add_to_print_queue(char *new_print_request);
void pop_from_print_queue(char *pooped_word);
