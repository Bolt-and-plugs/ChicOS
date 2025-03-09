#ifndef _CHICOS
#define _CHICOS

#include "defines.h"
#include "modules/log/log.h"
#include "modules/process/process.h"

typedef struct OS OS;

bool set_envvar(const char *mode);

int main(int argc, char **argv);

#endif
