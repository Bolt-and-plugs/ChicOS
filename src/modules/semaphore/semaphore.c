#include "semaphore.h"
#include "../process/process.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"

extern App app;

void waiter_push(semaphore *sem, u32 pid) {
  if ((sem->tail + 1) % DEFAULT_WAITERS_NUM == sem->head) {
    c_error(QUEUE_ERROR, "Waiters queue is full");
    return;
  }
  sem->waiters[sem->tail] = pid;
  sem->tail = (sem->tail + 1) % DEFAULT_WAITERS_NUM;
}

u32 waiter_pop(semaphore *sem) {
  if (sem->head == sem->tail) {
    c_error(QUEUE_EMPTY, "Waiters queue is empty");
    return -1;
  }
  u32 pid = sem->waiters[sem->head];
  sem->head = (sem->head + 1) % DEFAULT_WAITERS_NUM;
  return pid;
}

u32 get_waiters_size(semaphore *sem) {
  return (sem->tail + DEFAULT_WAITERS_NUM - sem->head) % DEFAULT_WAITERS_NUM;
}

void semaphoreP(semaphore *s, u32 pid) {
  if (!s) {
    c_error(SEMAPHORE_WAIT_ERROR, "Semaphore does not exist");
    return;
  }

  s->value--;
  if (s->value < 0) {
    waiter_push(s, pid);
    process *p = p_find(pid);
    p->status = WAITING;
  }
}

void semaphoreV(semaphore *s) {
  if (!s) {
    c_error(SEMAPHORE_POST_ERROR, "Semaphore does not exist");
    return;
  }

  s->value++;
  if (get_waiters_size(s) > 0) {
    u32 waking_pid = waiter_pop(s);
    process *waking_p = p_find(waking_pid);
    waking_p->status = READY;
  }
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
    app.semaphores->l[i].name = '\0';
    app.semaphores->l[i].waiters = NULL;
    app.semaphores->max = 0;
  }
}

int init_semaphore(char name, u32 value) {
  for (int i = 0; i < MAX_SIZE_SEMAPHORES; i++)
    if (app.semaphores->l[i].name == name) {
      c_warn(SEMAPHORE_INIT_ERROR, "Semaphore with name: %c already exists",
              name);
      return -1;
    }

  if (app.semaphores->last >= MAX_SIZE_SEMAPHORES) {
    c_error(SEMAPHORE_INIT_ERROR, "Semaphore list is full");
    return -1;
  }

  semaphore *sem = c_alloc(sizeof(semaphore));

  sem->name = name;
  sem->id = ++app.semaphores->max;
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
    if (app.semaphores->l[i].name == name)
      return &app.semaphores->l[i];
  }
  return NULL;
}
