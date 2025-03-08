#include "process.h"
#include "modules/log/log.h"

int p_create(process *process_list) {
  process p;
  p.parent_id = getpid();
  p.pid = p.parent_id + 1;
  return p.pid;
}

void log_process(i32 pid, process *process_list) {
  if (!process_list) {
    log(ERROR, 0, "s" , "No Process List", NULL);
    exit(ERROR);
  }
  log(INFO, 0, "s", "No Process List", NULL);
  process p;
  for (i32 i = 0; i < 10; i++) {
    if (process_list[i].pid == pid) {
      p = process_list[i];
      break;
    }
  }
  printf("Loggin process: %s\npid: %d\n", p.name, p.pid);
}
