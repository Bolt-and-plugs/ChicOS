#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../memory/arena.h"
#include "../utils/list.h"

#define MAX_SIZE_PROC_NAME 32
#define MAX_SIZE_SEMAPHORES 32

typedef enum {
  INACTIVE = -1,
  RUNNING = 0,
  READY = 1,
  BLOCKED = 2,
} p_status;

typedef struct __page_table {
  u32 base;
  u32 bounds;
  u8 magic_number;
} page_table;

typedef struct __process {
  const char *name;
  i32 pid;
  p_status status;
  i32 remaining_time;
  // yet to be done
  // understand how to impl segments here
  page_table pt;
  i32 tickets;
} process;

typedef struct __BCP {
  process processes[10];
} BCP;

int p_create(void);
void log_process(i32 pid);
void p_kill(i32 pid);

#endif
