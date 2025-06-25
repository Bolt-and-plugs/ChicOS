#include "process.h"
#include "../../chicos.h"
#include "../fs/synt.h"
#include "../log/log.h"
#include "../memory/mem.h"

extern App app;

void init_pcb(void) {
  app.pcb.last = 0;
  app.pcb.curr = 0;
  app.pcb.last_pid = 0;
  sem_init(&app.pcb.pcb_s, 0, 1);
}

void clear_pcb(void) {
  for (int i = 0; i < MAX_PCB; i++) {
    process *p = &app.pcb.process_stack[i];
    if (p->address_space) {
      c_dealloc(p->address_space);
    }
  }
}

events retrieve_event(const char *command) {
  events e;
  if (strcmp(command, "V") == 0)
    return semaphore_v;
  if (strcmp(command, "P") == 0)
    return semaphore_p;
  if (strcmp(command, "exec") == 0)
    return process_exec;
  if (strcmp(command, "write") == 0 || strcmp(command, "read") == 0)
    return disk_request;
  if (strcmp(command, "print") == 0)
    return print_request;
  return process_kill;
}

u32 get_pid(u32 seed) {
  bool exists = false;

  for (u8 i = 0; i < MAX_PCB; i++)
    if (seed == app.pcb.process_stack[i].pid)
      exists = true;

  return !exists ? app.cpu.quantum_time : get_pid(app.cpu.quantum_time + 1);
}

void init_code_section(process *p) {
  int i = 0;
  char aux[16], sem_aux[16], buff[16], *semaphore_name, *command;
  u32 time;

  if (p->fb->fp == NULL) {
    c_error(DISK_OPEN_ERROR, "Syntax buffer file not open properly!");
    return;
  }

  if (feof((p->fb->fp))) {
    sys_call(process_kill, "%u", p->pid);
    return;
  }

  p->c.it = (void *)p->address_space;
  p->c.PC = 0;
  p->c.size = (u32)sizeof(p->address_space) / INSTRUCTION_SIZE;

  fgets(aux, sizeof(aux), p->fb->fp);
  while (!feof(p->fb->fp)) {
    if (i * INSTRUCTION_SIZE >= p->c.size - 1) {
      c_error(CODE_SECTION_FAULT, "Code section for pid %u is too big!",
              p->pid);
      return;
    }

    if (!fgets(aux, sizeof(aux), p->fb->fp))
      break;
    strcpy(sem_aux, aux);
    command = strtok(sem_aux, "(");
    command = strtok(sem_aux, " ");

    instruction *c = &p->c.it[i++];

    c->fp_pos = ftell(p->fb->fp);
    c->e = retrieve_event(command);

    if (c->e == print_request || c->e == disk_request || c->e == process_exec) {
      time = atoi(strtok(NULL, " "));
      c->remaining_time = time;
      continue;
    }

    if (c->e == semaphore_v || c->e == semaphore_p) {
      semaphore_name = strtok(aux, "(");
      semaphore_name = strtok(NULL, "(");
      semaphore_name = strtok(semaphore_name, ")");
      c->sem_name = semaphore_name[0];
      continue;
    }

  }

  p->c.last = i;
}

u32 p_create(char *address) {
  char ret[16];
  char *name;
  char addr[MAX_ADDRESS_SIZE];

  strcpy(addr, address);

  name = strtok(address, "/");
  while (name != NULL) {
    strcpy(ret, name);
    name = strtok(NULL, "/");
  }
  name = strtok(ret, ".");

  if (app.pcb.curr >= MAX_PCB - 1) {
    c_error(PROCESS_CREATION_ERROR, "PCB is full");
    return -1;
  }

  process p = {.pid = app.pcb.last_pid++,
               .status = NEW,
               .address_space = NULL,
               .time_to_run = TIME_SLICE};
  memory_load_req(&p, KB);

  strcpy(p.name, name);
  p.fb = open_file(addr);

  init_code_section(&p);

  sem_wait(&app.pcb.pcb_s);
  app.pcb.process_stack[app.pcb.curr++] = p;
  app.pcb.last++;
  sem_post(&app.pcb.pcb_s);

  return p.pid;
}

void log_process(u32 pid) {
  process p;
  for (u32 i = 0; i < MAX_PCB; i++) {
    if (app.pcb.process_stack[i].pid == pid) {
      p = app.pcb.process_stack[i];
      break;
    }
  }

  if (!p.pid) {
    c_warn(PROCESS_OUT_OF_LIST, "No process with matching ID");
    return;
  }

  char res[256];
  char status[10];

  switch ((int)p.status) {
  case RUNNING:
    strcpy(status, "RUNNING");
    break;
  case BLOCKED:
    strcpy(status, "BLOCKED");
    break;
  case WAITING:
    strcpy(status, "WAITING");
    break;
  case NEW:
    strcpy(status, "NEW");
    break;
  case READY:
    strcpy(status, "READY");
    break;
  case KILL:
    strcpy(status, "KILL");
    break;
  }
  // DEBUG INFO
  char cut_name[121];
  strncpy(cut_name, p.name, 120);
  snprintf(res, 255, "process: %s\tpid: %d\tstatus %s\trw_count: %d\n",
           cut_name, p.pid, status, p.fb->h->rw_count);
  c_info(res);
}

process *p_find(u32 pid) {
  u8 retry = 5;

  while (retry-- > 0) {
    for (u32 i = 0; i < MAX_PCB; i++) {
      if (app.pcb.process_stack[i].pid == pid) {
        return &app.pcb.process_stack[i];
      }
    }
    usleep(100);
  }

  c_error(PROCESS_OUT_OF_LIST, "Process with PID %d not found", pid);
  return NULL;
}

void p_kill(u32 pid) {
  // to kill the process: free the memory allocated to it
  process *p = p_find(pid);

  if (!p) {
    c_error(PROCESS_OUT_OF_LIST, "Could not find process to kill");
    return;
  }

  sem_wait(&app.pcb.pcb_s);
  if (p->address_space)
    c_dealloc(p->address_space);
  p->status = KILL;
  sem_post(&app.pcb.pcb_s);
}

void p_interrupt(u32 pid) {
  process *p = p_find(pid);
  sem_wait(&app.pcb.pcb_s);
  p->time_to_run = 0;
  sem_post(&app.pcb.pcb_s);
}

void p_block(u32 pid) {
  process *p = p_find(pid);
  sem_wait(&app.pcb.pcb_s);
  p->status = BLOCKED;
  p->time_to_run = 0;
  sem_post(&app.pcb.pcb_s);
}

void p_unblock(u32 pid) {
  process *p = p_find(pid);
  sem_wait(&app.pcb.pcb_s);
  p->status = READY;
  sem_post(&app.pcb.pcb_s);
}
