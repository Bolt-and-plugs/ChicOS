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
  proc->status = READY;

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

void scheduler_no_running() {
  for (int i = 1; i < MAX_PCB; i++) {
    if (app.pcb.process_stack[i].status == RUNNING) {
      c_info("proccess %s (%d) was running and is now ready\n", app.pcb.process_stack->name, i);
      app.pcb.last = i;// ?????
      app.pcb.process_stack[i] = READY;
    }
  }
}

process *scheduler_get_process() {
  process *p = &app.pcb.process_stack[0];

  if (!p) {
    c_log(INFO, SCHEDULER_PROCESS_OUT_OF_BOUNDS, "acabou pro beta", NULL);
    return NULL;
  }

  for (int i = 1; i < MAX_PCB; i++) {
    if (p->fb->h->rw_count < app.pcb.process_stack[i]) {
      app.pcb.curr = i; // ????????
      p = &app.pcb.process_stack[i];
    }
  }
  p->status = RUNNING;

  return p;
}
