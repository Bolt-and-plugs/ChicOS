#ifndef _SCHEDULER
#define _SCHEDULER

#define MAX_PROC_SIZE 100

#include "../process/process.h"
#include "../../defines.h"

void push_process(process *proc);
process *pop_process(u32 pid);

#endif
