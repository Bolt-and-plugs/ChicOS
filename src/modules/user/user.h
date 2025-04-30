#ifndef __USER
#define __USER

typedef struct __user {
  char username[32];
  char password[32];
  bool logged;
} user;

void write_login_data(user u);
user *read_login_data(user u);

#endif
