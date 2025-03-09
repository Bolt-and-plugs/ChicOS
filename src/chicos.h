#ifndef _CHICOS
#define _CHICOS

#include "defines.h"
#include "modules/log/log.h"
#include "modules/process/process.h"

typedef struct OS OS;

struct OS {
  process process_list[10];
};

bool set_envvar(const char *mode);

int main(int argc, char **argv);

#endif
