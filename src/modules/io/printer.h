#ifndef __PRINTER
#define __PRINTER

#include "../../defines.h"

#define PRINTER_WINDOW 5

typedef struct __print_list {
  u32 time;
  u32 pid;
  struct __print_list *prox;
} print_list;

typedef struct __printer {
  pthread_t printer_t;
  sem_t printer_s;
  print_list *head;
  print_list *tail;
  print_list *printer_buffer;
  i16 buff_last;
  bool active;
} printer;

void *init_printer(void *args);
void add_to_print_queue(u32 time, u32 pid);
print_list *pop_from_print_queue();
void printer_loop();
print_list *get_print_time();

#endif
