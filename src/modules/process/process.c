#include "process.h"
#include "../log/log.h"
#include "chicos.h"

extern App *app

int p_create(PCB* pcb, int size) {
  // handle process creation
  if(pcb.curr == MAX_PCB-1){
    return -1; // erro, padronizar dps
  }
  process p = {
    .pid = getpid() + 1,
    .status = NEW,
    .address_space = create_arena(size)
  };
  pcb[pcb.curr] = p.pid;
  pcb.curr++;
  
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
    c_log(WARN, 404, "No process with matching ID", NULL);
    return;
  }

  printf("process: %s\npid: %d\n", p.name, p.pid);
}
