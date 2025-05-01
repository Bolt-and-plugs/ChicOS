#ifndef _CPU
#define _CPU

#include "../../defines.h"
#include "../semaphore/semaphore.h"

#define TIME_SLICE 10

typedef struct __cpu {
  u64 quantum_time;
  pthread_t cpu_t;
  sem_t cpu_s;
} cpu;

void *init_cpu(void *arg);

void cpu_loop();

void sys_call(events e, const char *str, ...);

#endif
