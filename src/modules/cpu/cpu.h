#ifndef _CPU
#define _CPU

#include "defines.h"

typedef struct __cpu {
  u64 quantum_time;
} cpu;

void init_cpu();

void cpu_loop();

#endif
