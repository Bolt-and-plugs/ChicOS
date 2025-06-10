#include "semaphore.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"

extern App app;

u32 waiter_get(u32 *waiters) {
  u32 pid;

  return pid;
}

void waiter_put(u32 *waiters, u32 pid) { return; }

u32 get_waiters_size(u32 *waiters) {
  u32 count = 0;

  return count;
}

void semaphoreP(semaphore *s, u32 pid) {
  if (!s) {
    c_error(SEMAPHORE_WAIT_ERROR, "Semaphore does not exist");
    return;
  }

  sem_wait(&app.semaphores->mutex);
  if (get_waiters_size(s->waiters) >= s->value) {
    waiter_put(s->waiters, pid);
    // waits here?
    sem_post(&app.semaphores->mutex);
  }

  s->value--;
  sem_post(&app.semaphores->mutex);
}

void semaphoreV(semaphore *s, u32 pid) {
  if (!s) {
    c_error(SEMAPHORE_POST_ERROR, "Semaphore does not exist");
    return;
  }
  sem_wait(&app.semaphores->mutex);
  s->value++;
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

int init_semaphore(char nome, u32 pid, u32 value) {
  for (int i = 0; i < MAX_SIZE_SEMAPHORES; i++)
    if (app.semaphores->l[i].nome == nome) {
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

  sem->owners = (u32 *)c_alloc(sizeof(u32) * (DEFAULT_OWNERS_NUM));
  sem->owners[0] = pid;
  sem->value = value;

  app.semaphores->l[app.semaphores->last] = *sem;
  app.semaphores->last++;
  return 0;
}
