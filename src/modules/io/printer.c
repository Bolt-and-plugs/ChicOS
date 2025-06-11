#include "printer.h"

#include "../log/log.h"
#include "../memory/mem.h"

print_list *head = NULL;
print_list *tail = NULL;
int count = 0;

void start_print_queue() {
  head = c_alloc(sizeof(print_list));
  head->prox = NULL;
  tail = head;
}

void add_to_print_queue(char *new_print_request) {
  print_list *new = c_alloc(sizeof(print_list));
  if (head == NULL) {
    //start_print_queue();
    strcpy(new->print_args, new_print_request);
    head = new;
    tail = new;
    return;
  } else {
    strcpy(new->print_args, new_print_request);
    tail->prox = new;
    tail = new;
  }
}

<<<<<<< HEAD
void pop_from_print_queue(char *popped_word) {
    if (head == NULL) {
        //c_error(QUEUE_EMPTY, "The print queue is empty");
        strcpy(popped_word, "");
        return;
    }

    print_list *to_del = head;

    strncpy(popped_word, to_del->print_args, 127);
    popped_word[127] = '\0';
    if(head == tail){
      if(head != NULL)
        c_dealloc(head);
      head = NULL;
      tail = NULL;
      return;
    }

    head = head->prox;
    if(to_del != NULL)
      c_dealloc(to_del);
=======
char *pop_from_print_queue() {
  if (head == NULL) {
    // c_error(QUEUE_EMPTY, "The print queue is empty");
    return NULL;
  }

  print_list *aux = head;
  char *popped_word = c_alloc(sizeof(char) * 128);

  strcpy(popped_word, head->print_args);
  if (head == tail)
    tail = tail->prox;
  head = head->prox;
  free(aux);
  return popped_word;
>>>>>>> b2991f4083164c43023f0c079ac9222075b1c500
}
