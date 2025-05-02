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

  for (int i = 0; i < MAX_PCB && !is_mem_free(processes[i].address_space);
       i++) {
    if (processes[i].status == READY) {
      for (int j = i + 1;
           j < MAX_PCB && !is_mem_free(processes[j].address_space); j++) {
        processes[j - 1] = processes[j];
      }

      processes[i].status = RUNNING;
      return &processes[i];
    }
  }

  c_error(SCHEDULER_PROCESS_OUT_OF_BOUNDS,
          "No process found at app.processes!");
  return NULL;
}

void scheduler_no_running() {
  for (int i = 1; i < MAX_PCB; i++) {
    if (!app.pcb.process_stack[i].address_space) {
      continue;
    }

    if (!is_mem_free(app.pcb.process_stack[i].address_space)) {
      app.pcb.last = i - 1;
      break;
    }

    if (app.pcb.process_stack[i].status == RUNNING) {
      c_info("proccess %s (%d) was running and is now ready\n",
             app.pcb.process_stack->name, i);
      app.pcb.process_stack[i].status = READY;
    }
  }
}

process *scheduler_get_process() {
  if (!app.pcb.process_stack[0].address_space) {
    c_info("No process currently running");
    return NULL;
  }

  process *p = &app.pcb.process_stack[0];

  if (!p) {
    c_error(SCHEDULER_PROCESS_OUT_OF_BOUNDS, "acabou pro beta");
    return NULL;
  }
  for (int i = 1; i < MAX_PCB; i++) {

    if (!is_mem_free(app.pcb.process_stack[i].address_space)) {
      app.pcb.last = i - 1;
      break;
    }

    if (app.pcb.process_stack[i].status == BLOCKED)
      continue;

    if (p->fb->h->rw_count < app.pcb.process_stack[i].fb->h->rw_count) {
      app.pcb.curr = (u8) i;
      p = &app.pcb.process_stack[i];
    }
  }

  p->status = RUNNING;

  return p;
}
