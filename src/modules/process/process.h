#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"

#define MAX_PCB 10
#define MAX_SIZE_PROC_NAME 32
#define MAX_SIZE_SEMAPHORES 32
#define QUANTUM_TIME (1000 / MAX_PCB)

typedef enum {
  NEW = -2,
  INACTIVE = -1,
  RUNNING = 0,
  READY = 1,
  BLOCKED = 2,
} p_status;

typedef enum {
  CODE = 0x0000,
  STACK = 0x0001,
  HEAP = 0x0002,
} segment;

typedef struct __page_table {
  u32 base;
  u32 bounds;
  u8 magic_number;
} page_table;

typedef struct __process {
  const char *name;
  u32 pid;
  p_status status;
  i32 time_to_run;
  page_table pt;
  void *address_space;
  u32 bounds;
  u16 qtd_read_write; // B.O. DO MÁRIO
} process;

typedef struct __PCB {
  process *processes_stack;
  u8 curr;
  i8 last;
} PCB;

void init_pcb(void);
u32 p_create(void);
void p_finish(void);
void log_process(u32 pid);
void p_kill(u32 pid);
void p_interrupt(u32 pid);
process *p_find(u32 pid);

#endif
