#include "cpu.h"
#include "../../chicos.h"
#include "../io/disk.h"
#include "../io/printer.h"
#include "../log/log.h"
#include "../utils/utils.h"

#include <stdio.h>
#include <string.h>
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
      sleep_ms(1000);
    else
      sleep_ms(5000);

    app.cpu.quantum_time++;

    scheduler_no_running();
    scheduler_kill_process();

    running_process = scheduler_get_process();
    if (running_process) {
      exec_program(running_process);
      log_process(running_process->pid);
    }

    if (app.cpu.quantum_time == 1 && app.debug)
      interrupt_control(process_create, "resources/sint1");

    if (app.cpu.quantum_time == 2 && app.debug)
      interrupt_control(process_create, "resources/sint1");

    if (app.cpu.quantum_time == 3 && app.debug)
      interrupt_control(process_create, "resources/sint3");

    if (app.cpu.quantum_time == 4 && app.debug)
      interrupt_control(process_create, "resources/sint4");

    if (app.cpu.quantum_time == 5 && app.debug)
      interrupt_control(process_create, "resources/sint1");

    if (app.cpu.quantum_time == 6 && app.debug)
      interrupt_control(process_create, "resources/sint2");

    if (app.cpu.quantum_time == 7 && app.debug)
      interrupt_control(process_create, "resources/sint3");

    if (app.cpu.quantum_time == 8 && app.debug)
      interrupt_control(process_create, "resources/sint4");

    if (app.cpu.quantum_time == 9 && app.debug)
      interrupt_control(process_create, "resources/sint1");

    if (app.cpu.quantum_time == 10 && app.debug)
      interrupt_control(process_create, "resources/sint2");

    if (app.cpu.quantum_time == 11 && app.debug)
      interrupt_control(process_create, "resources/sint3");

    if (app.cpu.quantum_time == 12 && app.debug)
      interrupt_control(process_create, "resources/sint4");
  }
}

void sys_call(events e, const char *str, ...) {
  sem_wait(&app.cpu.cpu_s);
  char buffer[MAX_ADDRESS_SIZE];
  u32 pid, time, bytes, track;
  void *ptr;
  char str_buf[DEFAULT_BUFFER_SIZE];
  char sem;

  va_list arg_list;
  va_start(arg_list, str);
  vsprintf(buffer, str, arg_list);
  va_end(arg_list);

  switch ((u8)e) {
  case disk_request:
    sscanf(buffer, "%u %u", &pid, &track);
    p_block(pid);
    simulate_io(pid, track);
    break;
  case process_interrupt:
    sscanf(buffer, "%u", &pid);
    p_interrupt(pid);
    break;
  case process_create:
    sscanf(buffer, "%s", str_buf);
    pid = p_create(str_buf);
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
    sscanf(buffer, "%c %u", &sem, &pid);
    semaphoreP(get_semaphore_by_name(sem), pid);
    break;
  case semaphore_v:
    sscanf(buffer, "%c", &sem);
    semaphoreV(get_semaphore_by_name(sem));
    break;
  case print_request:
    sscanf(buffer, "%u %u", &pid, &time);
    add_to_print_queue(time, pid);
    break;
  case print_finish:
    break;
  }
  sem_post(&app.cpu.cpu_s);
}

void interrupt_control(events e, const char *str, ...) {
  sem_wait(&app.cpu.cpu_s);
  char buffer[MAX_ADDRESS_SIZE];
  u32 time;
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
  case print_request:
    sscanf(buffer, "%u %u", &pid, &time);
    add_to_print_queue(time, pid);
    break;
  case print_finish:
    break;
  }
  sem_post(&app.cpu.cpu_s);
}

void exec_program(process *sint_process) {
  char *semaphore_name, *command, aux[16], sem_aux[16];
  u32 time;

  if (sint_process->fb->fp == NULL) {
    c_error(DISK_OPEN_ERROR, "File not open properly!");
    return;
  }

  while (!feof((sint_process->fb->fp)) || sint_process->time_to_run > 0) {
    if (!fgets(aux, sizeof(aux), sint_process->fb->fp)) {
      sys_call(process_kill, "%u", sint_process->pid);
      return;
    }

    strcpy(sem_aux, aux);
    command = strtok(sem_aux, "(");

    if (strcmp(command, "V") == 0) {
      semaphore_name = strtok(NULL, "(");
      semaphore_name = strtok(semaphore_name, ")");
      sys_call(semaphore_v, "%c", semaphore_name[0]);
    } else if (strcmp(command, "P") == 0) {
      semaphore_name = strtok(NULL, "(");
      semaphore_name = strtok(semaphore_name, ")");
      sys_call(semaphore_p, "%c %u", semaphore_name[0], sint_process->pid);
    } else {
      command = strtok(aux, " ");
      if (strcmp(command, "exec") == 0) {
        time = atoi(strtok(NULL, " "));
        u32 l_time = time > TIME_SLICE ? TIME_SLICE : time;
        sem_wait(&app.cpu.cpu_s);
        sleep_ms_with_time(l_time, &sint_process->time_to_run);
        if (time >= MAX_TIME_MORE_PAGES)
          sint_process->address_space = c_realloc(sint_process->address_space,
                                                  KB + (sizeof(page) * l_time));
        sem_post(&app.cpu.cpu_s);
        return;
      } else if (strcmp(command, "write") == 0) {
        sem_wait(&app.cpu.cpu_s);
        sint_process->fb->h->rw_count++;
        sem_post(&app.cpu.cpu_s);
        time = (u32)atoi(strtok(NULL, " "));
        sys_call(disk_request, "%u %u", sint_process->pid, time);
      } else if (strcmp(command, "read") == 0) {
        sem_wait(&app.cpu.cpu_s);
        sint_process->fb->h->rw_count++;
        sem_post(&app.cpu.cpu_s);
        time = (u32)atoi(strtok(NULL, " "));
        sys_call(disk_request, "%u %u", sint_process->pid, time);
      } else if (strcmp(command, "print") == 0) {
        time = (u32)atoi(strtok(NULL, " "));
        sys_call(print_request, "%u %u", sint_process->pid, time);
      } else {
        c_error(DISK_FILE_READ_ERROR,
                "Found invalid command!: %s in process %u", command,
                sint_process->pid);
      }
    }
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
