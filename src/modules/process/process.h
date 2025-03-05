#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"

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

struct process {
  i32 pid;
  i32 oid;
  i32 parent_id;
  char name[128];
  process *child;
  p_status status;
};

int p_create(i32 pid);
void log_process(i32 pid);
void p_kill(i32 pid);

#endif
