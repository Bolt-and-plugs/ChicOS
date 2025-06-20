#ifndef __USER
#define __USER

#include "../../defines.h"
#include <sodium.h>

#define MAX_LINE_LENGTH 256

typedef struct __user {
  char username[MAX_LINE_LENGTH];
  char password[crypto_pwhash_STRBYTES];
  bool logged;
} user;

void write_login_data(const user *u);
user *read_login_data(const user *u);
void retrieve_addr(const user *u, char *dest, size_t addr_sz);
void initialize_crypto();

#endif
