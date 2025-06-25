#include "cpu.h"
#include "../../chicos.h"
#include "../io/disk.h"
#include "../io/printer.h"
#include "../log/log.h"
#include "../utils/utils.h"

#include <stdio.h>

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
  while (!app.loop_stop) {
    if (app.debug)
      sleep_ms(1000);
    else
      sleep_ms(5000);

    app.cpu.quantum_time++;

    scheduler_no_running();
    scheduler_kill_process();

    process *running_process = scheduler_get_process();
    if (running_process) {
      exec_process(running_process);
      log_process(running_process->pid);
    }

    for (int i = 1; i <= 3; i++) {
      if (app.cpu.quantum_time == 1 * i && app.debug)
        interrupt_control(process_create, "resources/sint1");

      if (app.cpu.quantum_time == 2 * i && app.debug)
        interrupt_control(process_create, "resources/sint2");

      if (app.cpu.quantum_time == 3 * i && app.debug)
        interrupt_control(process_create, "resources/sint3");

      if (app.cpu.quantum_time == 4 * i && app.debug)
        interrupt_control(process_create, "resources/sint4");
    }
  }
}

void sys_call(const events e, const char *str, ...) {
  sem_wait(&app.cpu.cpu_s);
  char buffer[MAX_ADDRESS_SIZE];
  u32 pid, time, bytes, track;
  void *ptr;
  char str_buf[4096];
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
    pop_print();
    break;
  default:
    break;
  }
  sem_post(&app.cpu.cpu_s);
}

void interrupt_control(const events e, const char *str, ...) {
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
    pop_print();
    break;
  default:
    break;
  }
  sem_post(&app.cpu.cpu_s);
}

void exec_process(process *p) {
  if (p->time_to_run <= 0) {
    sys_call(process_interrupt, "%u", p->pid);
    return;
  }

  u32 l_time;
  bool control = false;
  for (; p->c.PC < p->c.last; p->c.PC++) {
    if (p->time_to_run <= 0 || control) {
      sys_call(process_interrupt, "");
    }
    switch ((events)p->c.it[p->c.PC].e) {
    case semaphore_p:
      sys_call(semaphore_p, "%c %u", p->c.it[p->c.PC].sem_name, p->pid);
      break;
    case semaphore_v:
      sys_call(semaphore_v, "%c %u", p->c.it[p->c.PC].sem_name, p->pid);
      break;
    case process_exec:

      if (p->c.it[p->c.PC].time_to_run > TIME_SLICE) {
        l_time = TIME_SLICE;
        p->c.it->time_to_run -= TIME_SLICE;
        p->c.PC--;
      } else
        l_time = p->c.it->time_to_run;

      sem_wait(&app.cpu.cpu_s);
      if (p->time_to_run > TIME_SLICE)
        control = true;
      else 
        sleep_ms_with_time(l_time, &p->time_to_run);
      if (p->c.it[p->c.PC].time_to_run >= MAX_TIME_MORE_PAGES)
        p->address_space =
            c_realloc(p->address_space, KB + (sizeof(page) * l_time));
      sem_post(&app.cpu.cpu_s);
      break;
    case disk_request:
      sem_wait(&app.cpu.cpu_s);
      p->fb->h->rw_count++;
      sem_post(&app.cpu.cpu_s);
      sys_call(disk_request, "%u %u", p->pid, p->c.it[p->c.PC].time_to_run);
      break;
    case print_request:
      sys_call(print_request, "%u %u", p->pid, p->c.it[p->c.PC].time_to_run);
      break;
    default:
      c_error(DISK_FILE_READ_ERROR, "Found invalid command in process %u",
              p->pid);
      break;
    }
  }
}
