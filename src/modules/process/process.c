#include "process.h"
#include "../log/log.h"
#include "chicos.h"

extern App *app;

void p_mem_allocate(process p, i8 qtd_pages) {
  // alloc qtd_pages pages to process p
}

int p_create() {
  process p = {.pid = getpid() + 1};
  // handle process creation (has its own page table)
  p_mem_allocate(p, 16);

  return p.pid;
}

void log_process(i32 pid) {
  process p;
  for (i32 i = 0; i < MAX_PROCESS; i++) {
    if (app->bcp.processes[i].pid == pid) {
      p = app->bcp.processes[i];
      break;
    }
  }

  if (!p.pid) {
    c_log(WARN, 404, "No process with matching ID", NULL);
    return;
  }

  printf("process: %s\npid: %d\n", p.name, p.pid);
}

process *find_p(i32 pid) {
  process *p = NULL;

  for (int i = 0; i < MAX_PROCESS; i++) {
    if (app->bcp.processes[i].pid == pid) {
      p = *app->bcp.processes[i];
      break;
    }
  }

  if (p == NULL) {
    c_log(ERROR, PROCESS_OUT_OF_LIST, "Proccess out of reach", NULL);
    return;
  }

  return p;
}

void p_kill(i32 pid) {
  // to kill the process: free the memory allocated to it
  process *p = find_p(pid);

  deallocate_mem(p->address_space);
}

void p_interrupt(i32 pid) {
  // interrupt occurs every time the quantum time goes to 0
  process *p = find_p(pid);

  p->time_to_run = QUANTUM_TIME;
}