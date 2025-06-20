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
  app.printer.printer_buffer = NULL;
  printer_loop();
  return NULL;
}

void printer_loop() {
  while (!app.loop_stop) {
    sleep_ms(5);

    if (app.printer.printer_buffer != NULL) {
      if (app.printer.printer_buffer->time > 0)
        app.printer.printer_buffer->time--;

      if (app.printer.printer_buffer->time == 0) {
        c_dealloc(app.printer.printer_buffer);
        app.printer.printer_buffer = NULL;
      }
    }

    if (app.printer.printer_buffer == NULL && app.printer.head != NULL)
      app.printer.printer_buffer = pop_from_print_queue();
  }
}

void add_to_print_queue(u32 time, u32 pid) {
  sem_wait(&app.printer.printer_s);
  print_list *new = c_alloc(sizeof(print_list));
  new->time = time;
  new->pid = pid;
  new->prox = NULL;

  if (app.printer.head == NULL) {
    app.printer.head = new;
    app.printer.tail = new;
  } else {
    app.printer.tail->prox = new;
    app.printer.tail = new;
  }

  sem_post(&app.printer.printer_s);
  return;
}

print_list *pop_from_print_queue() {
  sem_wait(&app.printer.printer_s);
  if (app.printer.head == NULL) {
    sem_post(&app.printer.printer_s);
    return NULL;
  }

  print_list *to_print = app.printer.head;
  app.printer.head = app.printer.head->prox;
  if (app.printer.head == NULL) {
    app.printer.tail = NULL;
  }

  sem_post(&app.printer.printer_s);
  return to_print;
}

print_list *get_print_time() {
  sem_wait(&app.printer.printer_s);
  print_list *buff = app.printer.printer_buffer;
  sem_post(&app.printer.printer_s);
  return buff;
}
