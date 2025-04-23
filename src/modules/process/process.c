#include "process.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"

extern App app;

void init_pcb(void) {
  app.pcb.processes_stack = malloc(sizeof(process) * MAX_PCB);
  printf("%ld", sizeof(app.pcb));
  app.pcb.last = 0;
  app.pcb.curr = 0;
}

u32 p_create(void) {
  // handle process creation
  if (app.pcb.curr == MAX_PCB - 1) {
    return -1; // erro, padronizar dps
  }
  process p = {.pid = getpid() + 1, .status = NEW, .address_space = alloc(KB)};
  app.pcb.processes_stack[app.pcb.curr] = p;
  app.pcb.curr++;
  return p.pid;
}

void log_process(u32 pid) {
  process p;
  for (u32 i = 0; i < MAX_PCB; i++) {
    if (app.pcb.processes_stack[i].pid == pid) {
      p = app.pcb.processes_stack[i];
      break;
    }
  }

  if (!p.pid) {
    c_warn(PROCESS_OUT_OF_LIST, "No process with matching ID");
    return;
  }

  printf("process: %s\npid: %d\n", p.name, p.pid);
}

process *p_find(u32 pid) {
  process *p = NULL;

  for (int i = 0; i < MAX_PCB; i++) {
    if (app.pcb.processes_stack[i].pid == pid) {
      p = &app.pcb.processes_stack[i];
      break;
    }
  }

  if (p == NULL) {
    c_error(PROCESS_OUT_OF_LIST, "Proccess out of reach");
    return NULL;
  }

  return p;
}

void p_kill(u32 pid) {
  // to kill the process: free the memory allocated to it
  process *p = p_find(pid);
  if (!p) {
    c_error(PROCESS_OUT_OF_LIST, "Could not find process");
    return;
  }

  dealloc(p->address_space);
  dealloc(p);
}

void p_interrupt(u32 pid) {
  // interrupt occurs every time the process quantum time goes to 0
  process *p = p_find(pid);

  p->time_to_run = QUANTUM_TIME;
}

void *p_alloc(process p, u32 bytes) {
  p.bounds += bytes;
  return (void *)((char *)p.address_space + p.bounds);
}
