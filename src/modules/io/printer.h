#include "../../defines.h"

typedef struct print_list print_list;

struct print_list
{
    char print_args[128];
    print_list *prox;
};

void start_print_queue();
void add_to_print_queue(char* new_print_request);
char* pop_to_print_queue();
