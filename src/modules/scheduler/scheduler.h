#ifndef _SCHEDULER
#define _SCHEDULER

#include "../process/process.h"
#include "../../defines.h"

process *scheduler_get_process();
void scheduler_no_running();
void scheduler_kill_process();

#endif
