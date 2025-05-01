#ifndef _CPU
#define _CPU

#include "../../defines.h"

#define TIME_SLICE 10

typedef struct __cpu {
  u64 quantum_time;
  pthread_t cpu_t;
} cpu;

void *init_cpu(void *arg);

void cpu_loop();

#endif
