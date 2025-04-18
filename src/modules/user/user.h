#ifndef _USER
#define _USER

#include "../../defines.h"

typedef struct __User {
  int id;
  char name[50];
} User;

void print_logo();

#endif
