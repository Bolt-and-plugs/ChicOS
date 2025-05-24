#include "disk.h"
#include "../../chicos.h"
#include "../cpu/cpu.h"
#include "../utils/utils.h"

extern App app;

void *init_disk(void *args) {
  sem_init(&app.disk.disk_s, 0, 1);
  app.disk.q.len = 0;
  disk_loop();
  return NULL;
}
void disk_loop() {
  while (!app.loop_stop) {
    usleep(10);
    if (app.disk.q.len > 0) {
      exec_io(&app.disk.q.queue[0]);
    }
  }
}

void simulate_io(u32 pid, u32 time_to_run) {
  io_req p = {.time_to_run = time_to_run, .id = pid};
  q_put(p);
}

void exec_io(io_req *p) {
  io_req local_p = *p;
  if (p->time_to_run > 0) {
    p->time_to_run--;
    return;
  }

  q_remove(local_p);
  interrupt_control(disk_finish, "%u", local_p.id);
}

void q_put(io_req p) {
  app.disk.q.queue[app.disk.q.len] = p;
  app.disk.q.len++;
}
void q_remove(io_req p) {
  int i;
  for (i = 0; i < app.disk.q.len - 1; i++) {
    app.disk.q.queue[i] = app.disk.q.queue[i + 1];
  }
  app.disk.q.queue[--app.disk.q.len] =
      (io_req){.time_to_run = 0, .id = 0};
}
