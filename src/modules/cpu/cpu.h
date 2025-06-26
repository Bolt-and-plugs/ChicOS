#ifndef _CPU
#define _CPU

#include "../../defines.h"
#include "../fs/synt.h"
#include "../process/process.h"
#include "../scheduler/scheduler.h"
#include "../semaphore/semaphore.h"

#define TIME_SLICE 1000
#define MAX_TIME_MORE_PAGES 1000


typedef struct __cpu {
  u64 quantum_time;
  pthread_t cpu_t;
  sem_t cpu_s;
} cpu;

void *init_cpu(void *arg);

void cpu_loop();

void sys_call(events e, const char *str, ...);

void interrupt_control(events e, const char *str, ...);

void exec_process(process *sint_process);

#endif
