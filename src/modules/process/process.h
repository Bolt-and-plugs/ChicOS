#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../memory/arena.h"

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
  const char *name;
  i32 pid;
  i32 parent_id;
  i32 tickets;
  process *child;
  p_status status;
  // Arena *address_space;
  // decision -> separate heap, stack and code? handle this all as linear
  // allocation?
  //
  // base and limit? even though this is hardware handled, we can implement it
  // here as a logical abstraction layer.
  // i32 base;
  // i32 bounds;
};

int p_create(void);
void log_process(i32 pid);
void p_kill(i32 pid);

#endif
