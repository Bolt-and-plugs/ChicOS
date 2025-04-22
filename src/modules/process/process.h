#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../memory/mem.h"
#include "../utils/list.h"

#define MAX_PCB 10
#define MAX_SIZE_PROC_NAME 32
#define MAX_SIZE_SEMAPHORES 32
#define MAX_SIZE_PROCESS 10
#define QUANTUM_TIME (1000 / MAX_PCB)

typedef enum {
  NEW = -2,
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
  i32 time_to_run;
  // yet to be done
  // understand how to impl segments here
  page_table pt;
  i32 tickets;
} process;

typedef struct __PCB {
  process processes[MAX_PCB];
  u8 curr;
} PCB;

void init_pcb(void);
i32 p_create(void);
void p_finish(void);
void log_process(i32 pid);
void p_interrut(i32 pid);
void p_kill(i32 pid);
void p_interrupt(i32 pid);

#endif
