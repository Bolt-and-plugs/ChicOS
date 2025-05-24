#ifndef _SEMAPHORE
#define _SEMAPHORE
#define MAX_SIZE_SEMAPHORES 32
#include "../../defines.h"
#include <semaphore.h>

typedef struct __semaphore {
  sem_t *s; // sem mais isso aqui @carlos pilantra
  char nome;
  u16 id;
} semaphore;

typedef struct __semaphore_list {
  semaphore l[MAX_SIZE_SEMAPHORES];
  int last;
} semaphore_list;

void semaphoreP(sem_t *s);
void semaphoreV(sem_t *s);
void init_semaphore_list();
void init_semaphore(char nome);

#endif
