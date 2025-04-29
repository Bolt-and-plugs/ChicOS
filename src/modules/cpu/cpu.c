#include "cpu.h"
#include "../../chicos.h"
#include "../io/file.h"
#include "../log/log.h"
#include "../process/process.h"
#include "../semaphore/semaphore.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern App app;

void cpu_loop() {
  u64 local_quantum_time = 0;
  while (!app.loop_stop) {
    if (local_quantum_time == UINT64_MAX - 1)
      local_quantum_time = 0;
    local_quantum_time++;
    if (local_quantum_time % 500 == 0) {
      app.cpu.quantum_time++;
      // loop de verdade
    }
    usleep(1000);
  }
}

void *init_cpu(void *arg) {
  app.cpu.quantum_time = 0;
  cpu_loop();
  return NULL;
}

void sys_call(events e, const char *str, ...) {
  va_list arg;
  va_start(arg, str);
  char args[4][16];
  int i = 0;
  while (str) {
    str = va_arg(arg, const char *);
    strcpy(args[i], str);
    printf("-> %s\n", str);
  }

  switch ((u8)e) {
  case process_interrupt:
    break;
  case process_create:
    p_create(args[0]);
    break;
  case process_finish:
    p_finish();
    break;
  case mem_load_req:
    break;
  case mem_load_finish:
    break;
  case semaphore_p:
    break;
  case semaphore_v:
    break;
  }
}

void exec_program(file_buffer *sint, process *sint_process) {
  char *semaphore, *command, aux[16];
  int time;
  if (sint->fp == NULL) {
    c_error(DISK_OPEN_ERROR, "File not open properly!");
    return;
  }

  for (int i = 0; i < 6; i++)
    fgets(aux, sizeof(aux), sint->fp);

  while (!feof(sint->fp)) {
    fgets(aux, sizeof(aux), sint->fp);
    command = strtok(aux, " ");
    if (strcmp(command, "exec") == 0) {
      time = atoi(strtok(NULL, " "));
      printf("Executing program for %dms...", time);
      sleep(time / 1000);
    } else if (strcmp(command, "write") == 0) {
      time = atoi(strtok(NULL, " "));
      printf("Writing on dik for %dms...", time);
      sleep(time / 1000);
    } else if (strcmp(command, "read") == 0) {
      time = atoi(strtok(NULL, " "));
      printf("Reading on dik for %dms...", time);
      sleep(time / 1000);
    } else if (strcmp(command, "P") == 0) {
      semaphore = strtok(NULL, " ");
      printf("Acessing critical storage session stored by %s", semaphore);
    } else if (strcmp(command, "V") == 0) {
      semaphore = strtok(NULL, " ");
      printf("Freeing critical storage session stored by %s", semaphore);
    } else if (strcmp(command, "print") == 0) {
    } else {
      c_error(DISK_FILE_READ_ERROR, "Found invalid command!");
    }
  }
}
