#include "disk.h"
#include "../../chicos.h"
#include "../cpu/cpu.h"
#include "../utils/utils.h"

extern App app;

void *init_disk(void *args) {
  sem_init(&app.disk.disk_s, 0, 1);
  app.disk.qr.len = 0;
  app.disk.current_track = 0;
  disk_loop();
  return NULL;
}

void sort_disk(u32 base) {
  u32 closest_idx = 0;
  u32 min_diff = abs_diff(base, app.disk.qr.queue[0].track);

  for (int i = 1; i < app.disk.qr.len; i++) {
    u32 current_diff = abs_diff(base, app.disk.qr.queue[i].track);
    if (current_diff < min_diff) {
      min_diff = current_diff;
      closest_idx = i;
    }
  }

  if (closest_idx != 0) {
    io_req temp = app.disk.qr.queue[0];
    app.disk.qr.queue[0] = app.disk.qr.queue[closest_idx];
    app.disk.qr.queue[closest_idx] = temp;
  }
}

void move_header(u32 target_track) {
  sem_wait(&app.disk.disk_s);
  if (target_track > app.disk.current_track) {
    app.disk.current_track++;
  }
  if (target_track < app.disk.current_track) {
    app.disk.current_track--;
  }
  sem_post(&app.disk.disk_s);
}

void disk_loop() {
  while (!app.loop_stop) {
    sleep_ms(1);
    if (app.disk.qr.len > 1)
      sort_disk(app.disk.current_track);
    if (app.disk.qr.len > 0) {
      if (app.disk.qr.queue[0].track == app.disk.current_track) {
        exec_io(&app.disk.qr.queue[0]);
        continue;
      }
      move_header(app.disk.qr.queue[0].track);
    }
  }
}

void simulate_io(u32 pid, u32 track) {
  if (track > TOTAL_TRACKS) {
    c_crit_error(DISK_MISS, "Track %u does not exists ", track);
    return;
  }

  io_req p = {.time_to_run = DISK_SLICE, .id = pid, .track = track};
  q_put(p);
}

void exec_io(io_req *p) {
  io_req local_p = *p;
  if (p->time_to_run > 0) {
    p->time_to_run--;
    app.disk.current_track = p->track;
    return;
  }

  q_remove(local_p);
  interrupt_control(disk_finish, "%u", local_p.id);
}

void q_put(io_req p) {
  app.disk.qr.queue[app.disk.qr.len] = p;
  app.disk.qr.len++;
}
void q_remove(io_req p) {
  int i;
  for (i = 0; i < app.disk.qr.len - 1; i++) {
    app.disk.qr.queue[i] = app.disk.qr.queue[i + 1];
  }
  app.disk.qr.queue[--app.disk.qr.len] = (io_req){.time_to_run = 0, .id = 0};
}
