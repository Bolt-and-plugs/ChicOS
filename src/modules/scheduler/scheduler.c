#include "scheduler.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../process/process.h"

extern App *app;

void push_process(
    process *proc) { // tem q ser de acordo com a quantidade de read e write
  process *processes = app->pcb.processes;
  proc->status = READY; // pode ser diferente no futuro

  if (!(app->pcb.last + 1 < MAX_PROC_SIZE)) {
    c_error(SCHEDULER_PROCESS_OUT_OF_BOUNDS, "PCB is full");
    return;
  }

  processes[app->pcb.last + 1] = *proc;
  app->pcb.last++;
  free(proc);
}

process *pop_process(u32 pid) {
  process *processes = app->pcb.processes;

  for (int i = 0; i < MAX_PROC_SIZE && processes[i].address_space != NULL;
       i++) {
    if (processes[i].status == READY) {
      for (int j = i + 1;
           j < MAX_PROC_SIZE && processes[j].address_space != NULL; j++) {
        processes[j - 1] = processes[j];
      }

      processes[i].status = RUNNING;
      return &processes[i];
    }
  }

  c_error(SCHEDULER_PROCESS_OUT_OF_BOUNDS, "No process found at app.processes");
  return NULL;
}
