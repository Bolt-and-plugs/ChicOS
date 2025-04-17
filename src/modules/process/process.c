#include "process.h"
#include "../log/log.h"

extern process process_list[10];

int p_create(void) {
  process p = {.pid = getpid() + 1};
  // handle process creation
  return p.pid;
}

void log_process(i32 pid) {
  process p;
  for (i32 i = 0; i < 10; i++) {
    if (process_list[i].pid == pid) {
      p = process_list[i];
      break;
    }
  }

  if (!p.pid) {
    c_warn(PROCESS_OUT_OF_LIST, "No process with matching ID");
    return;
  }

  printf("process: %s\npid: %d\n", p.name, p.pid);
}
