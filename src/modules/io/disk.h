#ifndef _DISK
#define _DISK

#include "../../defines.h"
#include "../process/process.h"

#define DISK_SLICE 4000
#define TOTAL_TRACKS 20000

typedef struct __io_req {
  u32 id;
  u32 time_to_run;
  u32 track;
} io_req;

typedef struct __queue_req {
  io_req queue[MAX_PCB];
  u8 len;
} queue_req;

typedef struct __disk {
  queue_req qr;
  pthread_t disk_t;
  sem_t disk_s;
  u32 current_track;
} disk;

void *init_disk(void *args);
void disk_loop();
void simulate_io(u32 pid, u32 track);
void exec_io(io_req *p);
void sort_disk(u32 base);
void q_put(io_req p);
void q_remove();

#endif
