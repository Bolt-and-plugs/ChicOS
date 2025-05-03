#ifndef _DISK
#define _DISK

#include "../../defines.h"
#include "../process/process.h"

typedef struct __blocked_process {
  u32 id;
  u32 time_to_run;
} blocked_process;

typedef struct __blocked_queue {
  blocked_process queue[MAX_PCB];
  u8 len;
} blocked_queue;

typedef struct __disk {
  blocked_queue q;
  pthread_t disk_t;
  sem_t disk_s;
} disk;

void *init_disk(void *args);
void disk_loop();
void simulate_io(u32 pid, u32 time_to_run);

void exec_io(blocked_process *p);

void q_put(blocked_process p);
void q_remove(blocked_process p);

#endif
