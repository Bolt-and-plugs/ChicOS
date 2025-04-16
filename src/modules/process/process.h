#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../memory/memory.h"
#include "../utils/list.h"

#define MAX_PCB 10

typedef enum {
  NEW = -2,
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
  void *address_space;
  i32 tickets;
};

struct BCP {
  process processes[MAX_PCB];
  u8 curr;
};

int p_create(void);
void p_finish(void);
void log_process(i32 pid);
void p_interrut(i32 pid);

#endif
