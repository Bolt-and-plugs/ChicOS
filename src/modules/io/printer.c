#include "printer.h"

#include "../log/log.h"
#include "../memory/mem.h"
#include "../semaphore/semaphore.h"

#include "../../defines.h"

print_list *head = NULL;
print_list *tail = NULL;
int count = 0;

void start_print_queue() {
  head = c_alloc(sizeof(print_list));
  head->prox = NULL;
  tail = head;
}

void add_to_print_queue(char *new_print_request) {
  if (head == NULL) {
    start_print_queue();
    strcpy(head->print_args, new_print_request);
  } else {
    print_list *new = c_alloc(sizeof(print_list));
    strcpy(new->print_args, new_print_request);
    tail->prox = new;
    tail = new;
  }
}

char* pop_from_print_queue() {
    if (head == NULL) {
        //c_error(QUEUE_EMPTY, "The print queue is empty");
        return NULL;
    }

    print_list *aux = head;
    char *popped_word = c_alloc(sizeof(char)*128);

    strcpy(popped_word, head->print_args);
    if(head == tail)
        tail = tail->prox;
    head = head->prox;
    free(aux);
    return popped_word;
}
