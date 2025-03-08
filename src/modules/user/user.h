#ifndef _USER
#define _USER

#include "../../defines.h"

typedef struct user user;

struct user {
  int id;
  char name[50];
};

#endif
