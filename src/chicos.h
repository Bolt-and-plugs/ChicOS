#ifndef _CHICOS
#define _CHICOS

#include "defines.h"
#include "modules/log/log.h"

typedef struct OS OS;

bool set_envvar(const char *mode);

void handle_args(int argc, char **argv);

int main(int argc, char **argv);

#endif
