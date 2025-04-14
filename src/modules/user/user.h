#ifndef _USER
#define _USER

#include "../../defines.h"

typedef struct __user User;

struct User {
  int id;
  char name[50];
};

void print_logo();

#endif
