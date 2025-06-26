#include "scheduler.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../process/process.h"

extern App app;

void scheduler_no_running() {
  for (int i = 0; i < app.pcb.last; i++) {
    sem_wait(&app.pcb.pcb_s);
    if (app.pcb.process_stack[i].status == RUNNING) {
      c_info("process %s (%d) was running and is now ready\n",
             app.pcb.process_stack->name, i);

      app.pcb.process_stack[i].status = READY;
    }
    sem_post(&app.pcb.pcb_s);
  }
}

process *scheduler_get_process() {
  if (app.pcb.last == 0) {
    c_info("No process currently running");
    return NULL;
  }

  process *selected = NULL;

  for (int i = 0; i < app.pcb.last; i++) {
    sem_wait(&app.pcb.pcb_s);
    process *candidate = &app.pcb.process_stack[i];

    if (candidate->status == BLOCKED || candidate->status == WAITING) {
      sem_post(&app.pcb.pcb_s);
      continue;
    }

    if (!selected || candidate->fb->h->rw_count < selected->fb->h->rw_count) {
      selected = candidate;
    } else if (candidate->h_used < selected->h_used &&
               candidate->fb->h->rw_count == selected->fb->h->rw_count) {
      selected = candidate;
    } else if (candidate->fb->h->rw_count == selected->fb->h->rw_count &&
               candidate->pid < selected->pid) {
      selected = candidate;
    }

    sem_post(&app.pcb.pcb_s);
  }

  if (!selected) {
    c_error(SCHEDULER_PROCESS_OUT_OF_BOUNDS, " No runnable process found");
    return NULL;
  }

  sem_wait(&app.pcb.pcb_s);
  selected->status = RUNNING;
  selected->time_to_run = TIME_SLICE;
  sem_post(&app.pcb.pcb_s);
  return selected;
}

void scheduler_kill_process() {
  for (int i = 0; i < MAX_PCB; i++) {
    process *candidate = &app.pcb.process_stack[i];

    if (candidate->status == KILL) {
      // alloc another process over the dead one
      for (int j = i + 1; j < MAX_PCB; j++) {
        sem_wait(&app.pcb.pcb_s);
        app.pcb.process_stack[j - 1] = app.pcb.process_stack[j];
        sem_post(&app.pcb.pcb_s);
      }
      sem_wait(&app.pcb.pcb_s);
      app.pcb.last--;
      sem_post(&app.pcb.pcb_s);
    }
  }
}
