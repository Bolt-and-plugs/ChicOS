#include "process.h"
#include "../log/log.h"
#include "chicos.h"

extern App *app

int p_create(PCB* pcb, int size) {
  // handle process creation
  if(pcb.curr == MAX_PCB-1){
    return -1; // erro, padronizar dps
  }
  process p = {
    .pid = getpid() + 1,
    .status = NEW,
    .address_space = create_arena(size)
  };
  pcb[pcb.curr] = p.pid;
  pcb.curr++;
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