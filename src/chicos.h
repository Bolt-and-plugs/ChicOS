#ifndef _CHICOS
#define _CHICOS

#include "defines.h"
#include "modules/cpu/cpu.h"
#include "modules/io/disk.h"
#include "modules/log/log.h"
#include "modules/memory/mem.h"
#include "modules/process/process.h"
#include "modules/render/render.h"
#include "modules/user/user.h"

typedef struct __App {
  PCB pcb;
  memory *mem;
  cpu cpu;
  disk disk;
  volatile sig_atomic_t loop_stop;
  log_level min_log_level;
  bool debug;
  renderer rdr;
  user *user;
  semaphore_list *semaphores;
} App;

void handle_signal(sig_atomic_t signal);

typedef enum { HELP = 1 } arguments;

bool set_envvar(const char *mode);

void handle_args(int *args, int argc, char **argv);

void init_app(int mem_size, bool should_render);

int main(int argc, char **argv);

#endif
