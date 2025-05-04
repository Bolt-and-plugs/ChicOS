#include "semaphore.h"
#include "../../chicos.h"
#include "../log/log.h"

extern App app;

void semaphoreP(sem_t *s) {
  if (!s) {
    c_error(SEMAPHORE_WAIT_ERROR, "Semaphore does not exist");
    return;
  }
  sem_wait(s);
}

void semaphoreV(sem_t *s) {
  if (!s) {
    c_error(SEMAPHORE_POST_ERROR, "Semaphore does not exist");
    return;
  }
  sem_post(s);
}

void init_semaphore_list() {
  app.semaphores = (semaphore_list*) malloc(sizeof(semaphore_list)); 
  if (!app.semaphores) {
    c_error(SEMAPHORE_INIT_ERROR, "Failed to initialize semaphore list");
    return;
  }
  app.semaphores->last = 0;
  for (int i = 0; i < MAX_SIZE_SEMAPHORES; i++) {
    app.semaphores->l[i]->id = 0;
    app.semaphores->l[i]->nome ='/0';
    app.semaphores->l[i]->s = NULL;
  }
}

void init_semaphore(char nome) {
  for (int i = 0; i < MAX_SIZE_SEMAPHORES; i++)
    if (app.semaphores->l[i]->nome == nome) {
      c_error(SEMAPHORE_INIT_ERROR, "Semaphore already exists");
      return;
    }
  
  if (app.semaphores->last >= MAX_SIZE_SEMAPHORES) {
    c_error(SEMAPHORE_INIT_ERROR, "Semaphore list is full");
    return;
  }

  semaphore *sem = malloc(sizeof(semaphore));

  sem->nome = nome;
  sem->id = app.cpu.quantum_time;
  sem->s = (sem_t*) malloc(sizeof(sem_t));

  sem_init(sem->s, 0, 1);

  app.semaphores->l[app.semaphores->last] = sem;
  app.semaphores->last++;
}
