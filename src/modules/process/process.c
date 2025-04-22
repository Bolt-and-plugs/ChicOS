#include "process.h"
#include "../../chicos.h"
#include "../log/log.h"

extern App *app;

void init_pcb(void) {
  // todo
}

i32 p_create(void) {
  // handle process creation
  if (app->pcb.curr == MAX_PCB - 1) {
    return -1; // erro, padronizar dps
  }
  process p = {.pid = getpid() + 1, .status = NEW, .address_space = alloc(KB)};
  app->pcb.processes[app->pcb.curr] = p;
  app->pcb.curr++;
  return p.pid;
}

void log_process(i32 pid) {
  process p;
  for (i32 i = 0; i < MAX_PCB; i++) {
    if (app->pcb.processes[i].pid == pid) {
      p = app->pcb.processes[i];
      break;
    }
  }

  if (!p.pid) {
    c_warn(PROCESS_OUT_OF_LIST, "No process with matching ID");
    return;
  }

  printf("process: %s\npid: %d\n", p.name, p.pid);
}

process *find_p(i32 pid) {
  process *p = NULL;

  for (int i = 0; i < MAX_PCB; i++) {
    if (app->pcb.processes[i].pid == pid) {
      p = &app->pcb.processes[i];
      break;
    }
  }

  if (p == NULL) {
    c_error(PROCESS_OUT_OF_LIST, "Proccess out of reach");
    return NULL;
  }

  return p;
}

void p_kill(i32 pid) {
  // to kill the process: free the memory allocated to it
  process *p = find_p(pid);
  if (!p) {
    c_error(PROCESS_OUT_OF_LIST, "Could not find process");
    return;
  }

  dealloc(p->address_space);
}

void p_interrupt(i32 pid) {
  // interrupt occurs every time the quantum time goes to 0
  process *p = find_p(pid);

  p->time_to_run = QUANTUM_TIME;
}
