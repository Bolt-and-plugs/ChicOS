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
  SLEEP = 3,
  AWAIT = 4,
  ZOMBIE = 5,
} p_status;

typedef struct process process;
typedef struct BCP BCP;

struct process {
  const char *name;
  i32 pid;
  i32 tickets;
  p_status status;
  i32 time_to_run;
  // yet to be done
  Arena *address_space;
};

struct BCP {
  List *processes;
};

int p_create(void);
void log_process(i32 pid);
void p_kill(i32 pid);

#endif
