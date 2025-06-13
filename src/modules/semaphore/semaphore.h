#ifndef _SEMAPHORE
#define _SEMAPHORE
#define MAX_SIZE_SEMAPHORES 32
#define DEFAULT_WAITERS_NUM 64
#define DEFAULT_OWNERS_NUM 64
#include "../../defines.h"
#include <semaphore.h>

typedef struct __semaphore {
  u16 id;
  char nome;
  u32 *waiters;
  u8 tail;
  u8 head;
  u8 value;
} semaphore;

typedef struct __semaphore_list {
  semaphore l[MAX_SIZE_SEMAPHORES];
  int last;
  sem_t mutex;
} semaphore_list;

void semaphoreP(semaphore *s, u32 pid);
void semaphoreV(semaphore *s, u32 pid);
void init_semaphore_list();
int init_semaphore(char nome, u32 value);
semaphore *get_semaphore_by_name(char name);

#endif
