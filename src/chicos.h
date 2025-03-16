#ifndef _CHICOS
#define _CHICOS

#include "defines.h"
#include "modules/log/log.h"
#include "modules/memory/arena.h"
#include "modules/process/process.h"
#include "modules/schedduler/schedduler.h"

typedef struct App App;

struct App {
  Arena virtua_mem;
  process *process_list;
};

bool set_envvar(const char *mode);

void handle_args(int *args, int argc, char **argv);

void init_app(int system_size);

int main(int argc, char **argv);

#endif
