#ifndef _DISK
#define _DISK

#include "../../defines.h"
#include "../process/process.h"

typedef struct __io_req {
  u32 id;
  u32 time_to_run;
} io_req;

typedef struct __queue_req {
  io_req queue[MAX_PCB];
  u8 len;
} queue_req;

typedef struct __disk {
  queue_req q;
  pthread_t disk_t;
  sem_t disk_s;
} disk;

void *init_disk(void *args);
void disk_loop();
void simulate_io(u32 pid, u32 time_to_run);
void exec_io(io_req *p);

// TODO change io logic -> SSTF
void q_put(io_req p);
void q_remove(io_req p);

#endif
