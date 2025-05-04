#include "cpu.h"
#include "../../chicos.h"
#include "../io/disk.h"
#include "../io/file.h"
#include "../log/log.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern App app;

void *init_cpu(void *arg) {
  if (arg)
    c_info(arg);
  app.cpu.quantum_time = 0;
  sem_init(&app.cpu.cpu_s, 0, 1);
  cpu_loop();
  return NULL;
}

void cpu_loop() {
  process *running_process;

  while (!app.loop_stop) {
    if (app.debug)
      usleep(100000);
    else
      sleep(1);

    app.cpu.quantum_time++;
    scheduler_no_running(); // troca o status "RUNNING" para "READY" em qualquer
                            // processo
    running_process = scheduler_get_process();
    if (running_process) {
      exec_program(running_process);
      log_process(running_process->pid);
    }

    interrupt_control(process_create, "resources/sint2");
  }
}

void sys_call(events e, const char *str, ...) {
  semaphoreP(&app.cpu.cpu_s);
  char buffer[MAX_ADDRESS_SIZE];
  u32 pid, time, bytes;
  void *ptr;

  va_list arg_list;
  va_start(arg_list, str);
  vsprintf(buffer, str, arg_list);
  va_end(arg_list);

  switch ((u8)e) {
  case disk_request:
    sscanf(buffer, "%u %u", &pid, &time);
    p_block(pid);
    simulate_io(pid, time);
    break;
  case process_interrupt:
    sscanf(buffer, "%u", &pid);
    p_interrupt(pid);
    break;
  case process_create:
    pid = p_create((char *)buffer);
    log_process(pid);
    break;
  case process_kill:
    sscanf(buffer, "%u", &pid);
    p_kill(pid);
    break;
  case mem_load_req:
    sscanf(buffer, "%p %u", &ptr, &bytes);
    memory_load_req(ptr, bytes);
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

void interrupt_control(events e, const char *str, ...) {
  semaphoreP(&app.cpu.cpu_s);
  char buffer[MAX_ADDRESS_SIZE];
  va_list arg_list;
  va_start(arg_list, str);
  vsprintf(buffer, str, arg_list);
  va_end(arg_list);

  u32 pid;
  void *ptr;
  switch ((u8)e) {
  case disk_finish:
    sscanf(buffer, "%u", &pid);
    p_unblock(pid);
    break;
  case process_interrupt:
    sscanf(buffer, "%u", &pid);
    p_interrupt(pid);
    break;
  case process_create:
    pid = p_create((char *)buffer);
    log_process(pid);
    break;
  case process_kill:
    sscanf(buffer, "%u", &pid);
    p_kill(pid);
    break;
  case mem_load_finish:
    sscanf(buffer, "%p", &ptr);
    memory_load_finish(ptr);
    break;
  }
  semaphoreV(&app.cpu.cpu_s);
}

void exec_program(process *sint_process) {
  char *semaphore, *command, aux[16], sem_aux[16];
  u32 time;
  if (sint_process->fb->fp == NULL) {
    c_error(DISK_OPEN_ERROR, "File not open properly!");
    return;
  }

  while (!feof((sint_process->fb->fp)) || sint_process->time_to_run >= 0) {
    fgets(aux, sizeof(aux), sint_process->fb->fp);
    strcpy(sem_aux, aux);
    command = strtok(sem_aux, "(");

    if (strcmp(command, "V") == 0) {
      semaphore = strtok(NULL, "(");
      printf("Acessing critical storage session stored by %s", semaphore);
    } else if (strcmp(command, "P") == 0) {
      semaphore = strtok(NULL, "(");
      semaphore = strtok(semaphore, ")");
      printf("Freeing critical storage session stored by %s", semaphore);
    } else {
      command = strtok(aux, " ");
      if (strcmp(command, "exec") == 0) {
        time = atoi(strtok(NULL, " "));
        wprintw(app.rdr.left_panel, "Executing program for %dms...", time);
        sleep(time / 1000);
      } else if (strcmp(command, "write") == 0) {
        sint_process->fb->h->rw_count++; // Contabiliza o rw_count

        time = atoi(strtok(NULL, " "));
        wprintw(app.rdr.left_panel, "Writing on disk for %ums...", time);
        sys_call(disk_request, "%d", sint_process->pid);
      } else if (strcmp(command, "read") == 0) {
        sint_process->fb->h->rw_count++; // Contabiliza o rw_count

        time = atoi(strtok(NULL, " "));
        wprintw(app.rdr.left_panel, "Reading on disk for %dms...", time);
        sys_call(disk_request, "%d", sint_process->pid);
      } else if (strcmp(command, "print") == 0) {
      } else {
        c_error(DISK_FILE_READ_ERROR, "Found invalid command!: %s", command);
      }
    }
    sint_process->time_to_run--;
    return;
  }

  // kill or interrupt process
  if (feof((sint_process->fb->fp))) {
    sys_call(process_kill, "%u", sint_process->pid);
    return;
  }

  if (sint_process->time_to_run <= 0) {
    sys_call(process_interrupt, "%u", sint_process->pid);
    return;
  }
}
