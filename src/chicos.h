#ifndef _CHICOS
#define _CHICOS

#include "defines.h"
#include "modules/memory/mem.h"
#include "modules/process/process.h"
#include "modules/cpu/cpu.h"

typedef struct __App {
  PCB pcb;
  memory *mem;
  cpu cpu;
} App;

typedef enum { HELP = 1 } arguments;

bool set_envvar(const char *mode);

void handle_args(int *args, int argc, char **argv);

void init_app(int system_size);

int main(int argc, char **argv);

#endif
