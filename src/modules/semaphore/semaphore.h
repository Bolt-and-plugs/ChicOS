#ifndef _SEMAPHORE
#define _SEMAPHORE

#include <semaphore.h>


typedef struct __semaphore {
  sem_t s;
  int id;
} semaphore;

typedef struct __semaphore_list {
  semaphore *l;
} semaphore_list;

void semaphoreP(sem_t *s);
void semaphoreV(sem_t *s);

#endif
