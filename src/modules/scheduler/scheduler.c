#include "scheduler.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../process/process.h"

extern App app;

/**
 * Tem q ser de acordo com a quantidade de read e write
 * 
 * Sempre reordena a pilha de processos tal q o processo
 * com a maior quantidade de read e write está em último
 */
void push_process(process *proc) {
  process *processes = app.pcb.process_stack;
  proc->status = READY; // pode ser diferente no futuro

  if (!(app.pcb.last + 1 < MAX_PCB)) {
    c_error(SCHEDULER_PROCESS_OUT_OF_BOUNDS, "PCB is full");
    return;
  }

  processes[++app.pcb.last] = *proc;
  free(proc);
}

/**
 * Remove o último elemento da pilha de processos e o retorna
 */
process *pop_process() {
  process *processes = app.pcb.process_stack;

  for (int i = 0; i < MAX_PCB && processes[i].address_space != NULL;
       i++) {
    if (processes[i].status == READY) {
      for (int j = i + 1;
           j < MAX_PCB && processes[j].address_space != NULL; j++) {
        processes[j - 1] = processes[j];
      }

      processes[i].status = RUNNING;
      return &processes[i];
    }
  }

  c_error(SCHEDULER_PROCESS_OUT_OF_BOUNDS, "No process found at app.processes!");
  return NULL;
}
