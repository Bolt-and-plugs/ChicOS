#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../memory/memory.h"
#include "../utils/list.h"

<<<<<<< HEAD
#define MAX_PCB 10
=======
#define MAX_SIZE_PROC_NAME 32
#define MAX_SIZE_SEMAPHORES 32
#define MAX_SIZE_PROCESS 10
#define QUANTUM_TIME (1000 / MAX_SIZE_PROCESS)
>>>>>>> origin/semaphore

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
  i32 time_to_run;
  // yet to be done
  // understand how to impl segments here
  void *address_space;
  i32 tickets;
};

struct BCP {
<<<<<<< HEAD
  process processes[MAX_PCB];
  u8 curr;
=======
  process processes[MAX_SIZE_PROCESS];
>>>>>>> origin/semaphore
};

int p_create(void);
void p_finish(void);
void log_process(i32 pid);
<<<<<<< HEAD
void p_interrut(i32 pid);
=======
void p_kill(i32 pid);
void p_interrupt(i32 pid);
>>>>>>> origin/semaphore

#endif
