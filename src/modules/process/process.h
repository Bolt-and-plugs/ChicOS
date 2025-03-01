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
typedef struct BCP BCP;

struct process {
  int pid;
  process *parent;
  p_status status;
  int oid;
};

int *p_create();
void p_kill(int pid);

#endif
