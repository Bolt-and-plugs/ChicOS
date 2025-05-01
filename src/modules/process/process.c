#include "process.h"
#include "../../chicos.h"
#include "../log/log.h"
#include "../memory/mem.h"

extern App app;

void init_pcb(void) {
  app.pcb.last = 0;
  app.pcb.curr = 0;
}

void clear_pcb(void) {
  for (int i = 0; i < MAX_PCB; i++) {
    process *p = &app.pcb.process_stack[i];
    if (p->address_space) {
      dealloc(p->address_space);
    }
  }
}

u32 p_create(char *address) {
  char ret[16];
  char *name;
  char addr[MAX_ADDRESS_SIZE];

  strcpy(addr, address);

  name = strtok(address, "/");
  while (name != NULL) {
    strcpy(ret, name);
    name = strtok(NULL, "/");
  }
  name = strtok(ret, ".");

  if (app.pcb.curr == MAX_PCB - 1) {
    c_error(PROCESS_CREATION_ERROR, "teste maroto fera");
    return -1;
  }

  process p = {.pid = app.cpu.quantum_time,
               .status = NEW,
               .address_space = alloc(KB),
               .time_to_run = TIME_SLICE};

  strcpy(p.name, name);
  p.fb = open_file(addr);

  app.pcb.process_stack[app.pcb.curr] = p;
  app.pcb.curr++;
  return p.pid;
}

void log_process(u32 pid) {
  process p;
  for (u32 i = 0; i < MAX_PCB; i++) {
    if (app.pcb.process_stack[i].pid == pid) {
      p = app.pcb.process_stack[i];
      break;
    }
  }

  if (!p.pid) {
    c_warn(PROCESS_OUT_OF_LIST, "No process with matching ID");
    return;
  }

  char res[256];
  snprintf(res, 255, "process: %s\npid: %d\n", p.name, p.pid);
  c_info(res);
}

process *p_find(u32 pid) {
  process *p = NULL;

  for (int i = 0; i < MAX_PCB; i++) {
    if (app.pcb.process_stack[i].pid == pid) {
      p = &app.pcb.process_stack[i];
      break;
    }
  }

  if (p == NULL) {
    c_error(PROCESS_OUT_OF_LIST, "Proccess out of reach");
    return NULL;
  }

  return p;
}

void p_kill(u32 pid) {
  // to kill the process: free the memory allocated to it
  process *p = p_find(pid);
  if (!p) {
    c_error(PROCESS_OUT_OF_LIST, "Could not find process");
    return;
  }

  if (p->address_space)
    dealloc(p->address_space);
}

void p_interrupt(u32 pid) {
  // interrupt occurs every time the process quantum time goes to 0
  process *p = p_find(pid);

  p->time_to_run = QUANTUM_TIME;
}
