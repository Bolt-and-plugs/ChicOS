#ifndef _PROCESS
#define _PROCESS

#include "../../defines.h"
#include "../fs/synt.h"

#define MAX_PCB 11
#define MAX_SIZE_PROC_NAME 32
#define INSTRUCTION_SIZE 16

typedef enum {
  NEW = -2,
  INACTIVE = -1,
  RUNNING = 0,
  READY = 1,
  BLOCKED = 2,
  KILL = 3,
  WAITING = 4,
} p_status;

typedef struct __instruction {
  events e;
  u32 remaining_time;
  u32 fp_pos;
  char sem_name;
} instruction;

typedef struct __code_section {
  instruction *it;
  u32 PC;
  u32 size;
  u32 last;
} code_section;

typedef struct __process {
  char name[MAX_ADDRESS_SIZE];
  u32 pid;
  p_status status;
  u32 time_to_run;
  u8 h_used;
  // memory
  void *address_space;
  // disk
  synt_buffer *fb;
  code_section c;
} process;

typedef struct __PCB {
  process process_stack[MAX_PCB];
  u8 curr;
  u8 last;
  u16 last_pid;
  sem_t pcb_s;
} PCB;

void init_pcb(void);
void clear_pcb(void);
u32 p_create(char *address);
void p_finish(void);
void log_process(u32 pid);
void p_kill(u32 pid);
void p_interrupt(u32 pid);
process *p_find(u32 pid);
void p_block(u32 pid);
void p_unblock(u32 pid);
void p_realloc(void *curr_region, u32 bytes, process *p);

#endif
