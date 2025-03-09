#include "process.h"
#include "../log/log.h"

extern process process_list[10];

int p_create(void) {
  process p = {.parent_id = getpid(), .pid = getpid() + 1};
  return p.pid;
}

void log_process(i32 pid) {
  process *p = malloc(sizeof(process));
  for (i32 i = 0; i < 10; i++) {
    if (process_list[i].pid == pid) {
      p = &process_list[i];
      break;
    }
  }

  if (!p) {
    log(WARN, 404, "No process with matching ID", NULL);
    return;
  }

  printf("process: %s\npid: %d\n", p->name, p->pid);
}
