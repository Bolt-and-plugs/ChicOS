#include "semaphore.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"

extern App app;

u32 waiter_get(u32 *waiters) {
  u32 pid;

  return pid;
}

void waiter_push(semaphore* sem, u32 pid) {
  if ((sem->tail + 1) % DEFAULT_WAITERS_NUM == sem->head) {
    c_error(QUEUE_ERROR,"Waiters queue is full");
    return;
  }
  sem->waiters[sem->tail] = pid;
  sem->tail = (sem->tail + 1) % DEFAULT_WAITERS_NUM;
}

u32 waiter_pop(semaphore* sem) {
  if (sem->head == sem->tail) {
    c_error(QUEUE_EMPTY,"Waiters queue is empty");
    return -1;
  }
  u32 pid = sem->waiters[sem->head];
  sem->head = (sem->head + 1) % DEFAULT_WAITERS_NUM;
  return pid;
}

u32 get_waiters_size(semaphore* sem){
  return (sem->tail + DEFAULT_WAITERS_NUM - sem->head) % DEFAULT_WAITERS_NUM;
}

void semaphoreP(semaphore *s, u32 pid) {
  if (!s) {
    c_error(SEMAPHORE_WAIT_ERROR, "Semaphore does not exist");
    return;
  }

  sem_wait(&app.semaphores->mutex);

  if (s->value > 0) {
    s->value--;  
  } else {
    waiter_push(s, pid);
    sem_wait(&app.pcb.pcb_s);
    app.pcb.process_stack[pid].status = BLOCKED;
    sem_post(&app.pcb.pcb_s);
  }

  sem_post(&app.semaphores->mutex);
}



void semaphoreV(semaphore *s, u32 pid) {
  if (!s) {
    c_error(SEMAPHORE_POST_ERROR, "Semaphore does not exist");
    return;
  }

  sem_wait(&app.semaphores->mutex);

  if (get_waiters_size(s) > 0) {
    u32 waking_pid = waiter_pop(s);
    sem_wait(&app.pcb.pcb_s);
    app.pcb.process_stack[waking_pid].status = READY;
    sem_post(&app.pcb.pcb_s);
  } else {
    s->value++;
  }

  sem_post(&app.semaphores->mutex);
}


void init_semaphore_list() {
  app.semaphores = (semaphore_list *)c_alloc(sizeof(semaphore_list));
  if (!app.semaphores) {
    c_error(SEMAPHORE_INIT_ERROR, "Failed to initialize semaphore list");
    return;
  }
  app.semaphores->last = 0;
  for (int i = 0; i < MAX_SIZE_SEMAPHORES; i++) {
    app.semaphores->l[i].id = 0;
    app.semaphores->l[i].nome = '\0';
    app.semaphores->l[i].waiters = NULL;
  }
}

int init_semaphore(char nome, u32 value) {
  for (int i = 0; i < MAX_SIZE_SEMAPHORES; i++)
    if (get_semaphore_by_name(nome) != NULL) {
      c_error(SEMAPHORE_INIT_ERROR, "Semaphore already exists");
      return -1;
    }

  if (app.semaphores->last >= MAX_SIZE_SEMAPHORES) {
    c_error(SEMAPHORE_INIT_ERROR, "Semaphore list is full");
    return -1;
  }

  semaphore *sem = c_alloc(sizeof(semaphore));

  sem->nome = nome;
  sem->id = app.cpu.quantum_time;
  sem->waiters = (u32 *)c_alloc(sizeof(u32) * (DEFAULT_WAITERS_NUM + value));
  sem->head = 0;
  sem->tail = 0;
  sem->value = value;

  app.semaphores->l[app.semaphores->last] = *sem;
  app.semaphores->last++;
  return 0;
}

semaphore *get_semaphore_by_name(char name) {
  for (int i = 0; i < MAX_SIZE_SEMAPHORES; i++) {
    if (app.semaphores->l[i].nome == name)
      return &app.semaphores->l[i];
  }
  return NULL;
}
