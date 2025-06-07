#include "printer.h"

#include "../log/log.h"
#include "../memory/mem.h"
#include "../semaphore/semaphore.h"

#include "../../defines.h"

print_list *head = NULL;
print_list *tail = NULL;

void start_print_queue()
{
    head = c_alloc(sizeof(print_list));
    head->prox = NULL;
    tail = head;
}

void add_to_print_queue(char* new_print_request)
{
    if(head == NULL)
    {
        start_print_queue();
        strcpy(head->print_args, new_print_request);   
    }
    else
    {
        print_list *new = c_alloc(sizeof(print_list));
        strcpy(new->print_args, new_print_request);
        tail->prox = new;
        tail = new;
    }
}

char* pop_from_print_queue()
{
    if(head == NULL)
    {
        //c_error()
        return NULL;
    }

    print_list *curr = head, *past = curr;

    while(curr != tail)
    {
        past = curr;
        curr = curr->prox;
    }

    past->prox = NULL;
    char popped_word[128];
    strcpy(popped_word, tail->print_args);
    tail = past;
    free(curr);
    return popped_word;
}