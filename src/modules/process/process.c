#include "process.h"
#include "../../chicos.h"
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

u32 get_pid(u32 seed) {
  bool exists = false;

  for (u8 i = 0; i < MAX_PCB; i++)
    if (seed == app.pcb.process_stack[i].pid)
      exists = true;

  return !exists ? app.cpu.quantum_time : get_pid(app.cpu.quantum_time + 1);
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
  snprintf(res, 255, "process: %s\npid: %d\nstatus %s\nrw_count: %d\n", p.name,
           p.pid, status, p.fb->h->rw_count);
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
  // interrupt occurs every time the process quantum time goes to 0
  process *p = p_find(pid);

  sem_wait(&app.pcb.pcb_s);
  // todo processo é incializado com time_to_run = TIME_SLICE
  // p->time_to_run = QUANTUM_TIME;
  sem_post(&app.pcb.pcb_s);
}

void p_block(u32 pid) {
  process *p = p_find(pid);

  sem_wait(&app.pcb.pcb_s);
  p->status = BLOCKED;
  sem_post(&app.pcb.pcb_s);
}

void p_unblock(u32 pid) {
  process *p = p_find(pid);
  sem_wait(&app.pcb.pcb_s);
  p->status = READY;
  sem_post(&app.pcb.pcb_s);
}
