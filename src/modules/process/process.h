#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../io/file.h"

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
  CODE = 0x000,
  STACK = 0x001,
  HEAP = 0x002,
} segment;

typedef struct __page_table {
  u32 base;
  u32 bounds;
  u8 magic_number;
} page_table;

typedef struct __process {
  u32 pid;
  const char *name;
  p_status status;
  u32 time_to_run;

  // memory
  page_table pt;
  void *address_space;

  // quantidade de read e write pro scheduler poder determinar quem vai ser
  // executado primeiro
  u16 rw_count;
  file_buffer *fb;
} process;

typedef struct __PCB {
  process *process_stack;
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
