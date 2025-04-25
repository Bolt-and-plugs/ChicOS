#include "../io/file.h"
#include "../log/log.h"
#include "../process/process.h"
#include "../../chicos.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define TIME_SLICE 10

extern App app;

void cpu_loop() {
  while (!app.loop_stop) {
    void *ptr = alloc(150);
    sleep(1);
    dealloc(ptr);
  }
}

void *init_cpu(void *arg) {
  app.cpu.quantum_time = 0;
  cpu_loop();
  return NULL;
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
