#ifndef _MAIN 
#define _MAIN

#include "defines.h"
#include "modules/process/process.h"

typedef struct OS OS;

struct OS {
  process process_list[10];
};

int main(int argc, char **argv);

#endif
