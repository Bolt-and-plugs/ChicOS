#include "printer.h"

#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include "../utils/utils.h"

extern App app;

void *init_printer(void *args) {
  sem_init(&app.printer.printer_s, 0, 1);
  app.printer.print_event_buff = c_alloc(4096);
  printer_loop();
  return NULL;
}

void printer_loop() {
  int i = 0;
  char *print_buff = c_alloc(4096);
  while (!app.loop_stop) {
    sleep_ms(5);
    if (app.printer.head != NULL) {
      while (app.printer.tail != NULL) {
        pop_from_print_queue(print_buff);
        mvwprintw(app.rdr.left_bottom, i + 2, 1, "%s", print_buff);
      }
      c_dealloc(print_buff);
    }
  }
}

void init_print_queue() {
  app.printer.head = c_alloc(sizeof(print_list));
  app.printer.head->prox = NULL;
  app.printer.tail = app.printer.head;
}

void add_to_print_queue(char *new_print_request, u32 time) {
  print_list *new = c_alloc(sizeof(print_list));
  if (app.printer.head == NULL) {
    // init_print_queue();
    strcpy(new->print_args, new_print_request);
    app.printer.head = new;
    app.printer.tail = new;
    return;
  } else {
    strcpy(new->print_args, new_print_request);
    app.printer.tail->prox = new;
    app.printer.tail = new;
  }
}

void pop_from_print_queue(char *popped_word) {
  if (app.printer.head == NULL) {
    c_error(QUEUE_EMPTY, "The print queue is empty");
    strcpy(popped_word, "");
    return;
  }

  print_list *to_del = app.printer.head;

  strncpy(popped_word, to_del->print_args, 127);
  popped_word[127] = '\0';
  if (app.printer.head == app.printer.tail) {
    if (app.printer.head != NULL)
      c_dealloc(app.printer.head);
    app.printer.head = NULL;
    app.printer.tail = NULL;
    return;
  }

  app.printer.head = app.printer.head->prox;
  if (to_del != NULL)
    c_dealloc(to_del);
}
