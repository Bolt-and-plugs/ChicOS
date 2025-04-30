#include "user.h"
#include "../../defines.h"
#include "../io/file.h"
#include "../memory/mem.h"

void retrieve_addr(user u, char *addr) {
  strcat(addr, "data/");
  strcat(addr, u.username);
}

char *encrypt(char *password) { return password; }

void write_login_data(user u) {
  char addr[MAX_ADDRESS_SIZE];
  retrieve_addr(u, addr);
  printf("%s\n", addr);
  FILE *fp = fopen(addr, "w");

  fprintf(fp, "user: %s\n pswd:%s\n", u.username, encrypt(u.password));
}

char *decrypt(char *password) { return password; }

user *read_login_data(user u) {
  char addr[MAX_ADDRESS_SIZE];
  retrieve_addr(u, addr);
  FILE *fp = fopen(addr, "r");

  user *local_user = alloc(sizeof(user));
  fscanf(fp, "user: %s\n pswd:%s\n", local_user->username,
         local_user->password);

  if (strcmp(local_user->username, u.username) &&
      strcmp(decrypt(local_user->password), u.password)) {
    return local_user;
  }

  dealloc(local_user);
  return NULL;
}
