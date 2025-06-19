#include "printer.h"

#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"
#include "../utils/utils.h"

extern App app;

void *init_printer(void *args) {
  sem_init(&app.printer.printer_s, 0, 1);
  app.printer.head = c_alloc(sizeof(print_list));
  app.printer.head->prox = NULL;
  app.printer.tail = app.printer.head;
  app.printer.active = true;
  app.printer.buff_last = -1;
  printer_loop();
  return NULL;
}

void printer_loop() {
  char *local_print_buff = c_alloc(MAX_PRINTER_OUTPUT);
  u32 *time_print_buff = c_alloc(sizeof(u32));
  while (!app.loop_stop) {
    int i=0;
    sleep_ms(5);
    if (app.printer.head != NULL && i < PRINTER_WINDOW) {
      while (app.printer.head != NULL && i < PRINTER_WINDOW) {
        pop_from_print_queue(local_print_buff, time_print_buff);
        strcpy(app.printer.printer_buff[i], local_print_buff);
        app.printer.printer_time_buff[i] = *time_print_buff;
        app.printer.printer_time_buff[i]--;
        app.printer.buff_last++;
        i++;
      }
    }
    // else {
    //   i = 0;
    //   while(i <= app.printer.buff_last) {
    //     app.printer.printer_time_buff[i]--;
    //     i++;
    //   }
    // }
    c_dealloc(local_print_buff);
    c_dealloc(time_print_buff);
  }
}

void add_to_print_queue(char *new_print_request, u32 time) {
  print_list *new = c_alloc(sizeof(print_list));
  if (app.printer.head == NULL) {
    strcpy(new->print_args, new_print_request);
    new->time = time;
    app.printer.head = new;
    app.printer.tail = new;
    return;
  } else {
    strcpy(new->print_args, new_print_request);
    new->time = time;
    app.printer.tail->prox = new;
    app.printer.tail = new;
  }
}

void pop_from_print_queue(char *popped_word, u32 *time_to_print) {
  if (app.printer.head == NULL) {
    c_error(QUEUE_EMPTY, "The print queue is empty");
    strcpy(popped_word, "");
    *time_to_print = -1;
    return;
  }

  print_list *to_del = app.printer.head;

  strncpy(popped_word, to_del->print_args, 127);
  popped_word[127] = '\0';
  *time_to_print = to_del->time;
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
