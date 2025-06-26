#include "printer.h"

#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include "../utils/utils.h"

extern App app;

void *init_printer(void *args) {
  sem_init(&app.printer.printer_s, 0, 1);
  app.printer.head = NULL;
  app.printer.tail = NULL;
  app.printer.active = true;
  printer_loop();
  return NULL;
}

void printer_loop() {
  while (!app.loop_stop) {
  sleep_ms(5);
    if (app.printer.head != NULL) {
      app.printer.printer_time_buff = pop_from_print_queue();
    }
    if(app.printer.printer_time_buff > 0) {
      app.printer.printer_time_buff--;
      continue;
    }
  }
}

void add_to_print_queue(u32 time) {
  print_list *new = c_alloc(sizeof(print_list));
  if (app.printer.head == NULL) {
    app.printer.head = c_alloc(sizeof(print_list));
    new->time = time;
    app.printer.head = new;
    app.printer.tail = new;
    return;
  }
  new->time = time;
  app.printer.tail->prox = new;
  app.printer.tail = new;
  return;
}

u32 pop_from_print_queue() {
  if (app.printer.head == NULL) {
    c_error(QUEUE_EMPTY, "The print queue is empty");
    return -1;
  }

  print_list *to_del = app.printer.head;

  u32 time_to_print = to_del->time;
  if (app.printer.head == app.printer.tail) {
    if (app.printer.head != NULL)
      c_dealloc(app.printer.head);
    app.printer.head = NULL;
    app.printer.tail = NULL;
    return time_to_print;
  }

  app.printer.head = app.printer.head->prox;
  if (to_del != NULL)
    c_dealloc(to_del);
  return time_to_print;
}
