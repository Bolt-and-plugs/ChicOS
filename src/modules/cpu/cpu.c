#include "cpu.h"
#include "../../chicos.h"
#include "../io/file.h"
#include "../log/log.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern App app;

void cpu_loop() {
  process *running_process;

  while (!app.loop_stop) {
    if (app.debug)
      usleep(1000000);
    else
      sleep(1);

    app.cpu.quantum_time++;
    scheduler_no_running();
    running_process = scheduler_get_process();
    if (running_process)
      exec_program(running_process);
  }
}

void *init_cpu(void *arg) {
  app.cpu.quantum_time = 0;
  sem_init(&app.cpu.cpu_s, 0, 1);
  cpu_loop();
  return NULL;
}

void sys_call(events e, const char *str, ...) {
  semaphoreP(&app.cpu.cpu_s);
  char buffer[4096];
  va_list arg_list;
  va_start(arg_list, str);
  vsprintf(buffer, str, arg_list);
  va_end(arg_list);

  int pid;
  process *p;
  switch ((u8)e) {
  case process_interrupt:
    p_interrupt(*(int *)buffer);
    break;
  case process_create:
    pid = p_create((char *)buffer);
    log_process(pid);
    break;
  case process_kill:
    p_kill(*(int *)(buffer));
    break;
  case mem_load_req:
    break;
  case mem_load_finish:
    break;
  case semaphore_p:
    semaphoreP((sem_t *)buffer);
    break;
  case semaphore_v:
    semaphoreV((sem_t *)buffer);
    break;
  }
  semaphoreV(&app.cpu.cpu_s);
}

void exec_program(process *sint_process) {
  char *semaphore, *command, aux[16];
  int time;
  if (sint_process->fb->fp == NULL) {
    c_error(DISK_OPEN_ERROR, "File not open properly!");
    return;
  }

  for (int i = 0; i < 6; i++)
    fgets(aux, sizeof(aux), sint_process->fb->fp);

  while (!feof((sint_process->fb->fp)) || sint_process->time_to_run <= 0) {
    fgets(aux, sizeof(aux), sint_process->fb->fp);
    command = strtok(aux, " ");
    if (strcmp(command, "exec") == 0) {
      time = atoi(strtok(NULL, " "));
      printf("Executing program for %dms...", time);
      sleep(time / 1000);
    } else if (strcmp(command, "write") == 0) {
      time = atoi(strtok(NULL, " "));
      printf("Writing on dik for %dms...", time);
      sleep(time / 1000);
    } else if (strcmp(command, "read") == 0) {
      time = atoi(strtok(NULL, " "));
      printf("Reading on dik for %dms...", time);
      sleep(time / 1000);
    } else if (strcmp(command, "P") == 0) {
      semaphore = strtok(NULL, " ");
      printf("Acessing critical storage session stored by %s", semaphore);
    } else if (strcmp(command, "V") == 0) {
      semaphore = strtok(NULL, " ");
      printf("Freeing critical storage session stored by %s", semaphore);
    } else if (strcmp(command, "print") == 0) {
    } else {
      c_error(DISK_FILE_READ_ERROR, "Found invalid command!");
    }
    sint_process->time_to_run--;
  }

  // kill or interrupt process
  if (feof((sint_process->fb->fp)))
    sys_call(process_kill, "%d", sint_process->pid);

  if (sint_process->time_to_run <= 0)
    sys_call(process_interrupt, "%d", sint_process->pid);
}
