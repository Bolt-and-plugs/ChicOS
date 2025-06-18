#ifndef __PRINTER
#define __PRINTER

#include "../../defines.h"
#include "ncurses.h"

#define MAX_PRINTER_OUTPUT 4096
#define PRINTER_WINDOW 5

typedef struct __print_list {
  char print_args[MAX_PRINTER_OUTPUT];
  u32 time;
  struct __print_list *prox;
} print_list;

typedef struct __printer {
  print_list queue;
  pthread_t printer_t;
  sem_t printer_s;
  print_list *head;
  print_list *tail;
  char printer_buff[PRINTER_WINDOW][MAX_PRINTER_OUTPUT];
  u8 buff_last;
} printer;

void *init_printer(void *args);
void init_print_queue();
void add_to_print_queue(char *new_print_request, u32 time);
void pop_from_print_queue(char *pooped_word);
void printer_loop();

#endif
