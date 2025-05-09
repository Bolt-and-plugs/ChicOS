#include "scheduler.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../process/process.h"

extern App app;

void scheduler_no_running() {
  for (int i = 1; i < MAX_PCB; i++) {
    if (!app.pcb.process_stack[i].address_space) {
      continue;
    }

    if (!is_mem_free(app.pcb.process_stack[i].address_space))
      break;

    if (app.pcb.process_stack[i].status == RUNNING) {
      c_info("proccess %s (%d) was running and is now ready\n",
             app.pcb.process_stack->name, i);
      semaphoreP(&app.pcb.pcb_s);
      app.pcb.process_stack[i].status = READY;
      semaphoreV(&app.pcb.pcb_s);
    }
  }
}

process *scheduler_get_process() {
  if (!app.pcb.process_stack[0].address_space) {
    return NULL;
  }

  process *selected = NULL;

  for (int i = 0; i < MAX_PCB; i++) {
    process *candidate = &app.pcb.process_stack[i];

    if (!candidate->address_space || is_mem_free(candidate->address_space))
      continue;

    if (candidate->status == BLOCKED)
      continue;

    if (!selected || candidate->fb->h->rw_count > selected->fb->h->rw_count) {
      selected = candidate;
    } else if (candidate->fb->h->rw_count == selected->fb->h->rw_count &&
               candidate->pid < selected->pid) {
      // nesse caso selected existe e seu rw_count é menor ou igual o de
      // candidate, além do óbvio
      selected = candidate;
    }
  }

  if (!selected) {
    c_debug(SCHEDULER_PROCESS_OUT_OF_BOUNDS, "No runnable process found");
    return NULL;
  }

  selected->status = RUNNING;
  selected->time_to_run = TIME_SLICE;
  return selected;
}

void scheduler_kill_process() {
  for (int i = 0; i < MAX_PCB; i++) {
    process *candidate = &app.pcb.process_stack[i];

    if (candidate->status == KILL) {
      // aloca os outros processos por cima do processo morto
      for (int j = i + 1; j < MAX_PCB; j++) { // n sei se da mem leak
        semaphoreP(&app.pcb.pcb_s);
        app.pcb.process_stack[j - 1] = app.pcb.process_stack[j];
        semaphoreV(&app.pcb.pcb_s);
      }
      semaphoreP(&app.pcb.pcb_s);
      app.pcb.last--;
      semaphoreV(&app.pcb.pcb_s);
    }
  }
}
