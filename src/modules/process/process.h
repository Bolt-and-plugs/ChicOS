#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../memory/arena.h"
#include "../utils/list.h"

typedef enum {
  INACTIVE = -1,
  RUNNING = 0,
  READY = 1,
  BLOCKED = 2,
} p_status;

typedef struct process process;
typedef struct BCP BCP;

struct process {
  const char *name;
  i32 pid;
  p_status status;
  i32 remaining_time;
  // yet to be done
  // understand how to impl segments here
  Arena *address_space;
  i32 tickets;
};

struct BCP {
  process processes[10];
};

int p_create(void);
void log_process(i32 pid);
void p_kill(i32 pid);

#endif
