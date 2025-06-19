#ifndef __USER
#define __USER

#include "../../defines.h"

#define MAX_LINE_LENGTH 64

typedef struct __user {
  char username[MAX_LINE_LENGTH];
  char password[MAX_LINE_LENGTH];
  bool logged;
} user;

void write_login_data(const user *u);
user *read_login_data(const user *u);
void retrieve_addr(const user *u, char *dest, size_t addr_sz);
void initialize_crypto();

#endif
