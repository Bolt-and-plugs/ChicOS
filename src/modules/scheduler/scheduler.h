#ifndef _SCHEDULER
#define _SCHEDULER

#include "../process/process.h"
#include "../../defines.h"

void push_process(process *proc);
process *pop_process();

#endif
