#ifndef __USER
#define __USER

#include "../../defines.h"

typedef struct __user {
  char username[32];
  char password[32];
  bool logged;
} user;

void write_login_data(const user *u);
user *read_login_data(const user *u);
void retrieve_addr(const user *u, char *dest, size_t addr_sz);

#endif
