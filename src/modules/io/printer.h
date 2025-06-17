#include "../../defines.h"
#include "ncurses.h"

typedef struct __print_list print_list;
typedef struct __printer printer;

struct __print_list {
  char print_args[4096];
  u32 time;
  print_list *prox;
};

struct __printer {
  print_list queue;
  pthread_t printer_t;
  sem_t printer_s;
  print_list *head;
  print_list *tail;
  char *print_event_buff;
};

void init_print_queue();
void add_to_print_queue(char *new_print_request, u32 time);
void pop_from_print_queue(char *pooped_word);
void printer_loop();